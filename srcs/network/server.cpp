/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 22:21:29 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/29 01:21:39 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/networking.hpp"
#include "core/threading.hpp"
#include <cstring>
#include <arpa/inet.h>
#include <fcntl.h>

Server::Server() : _sock(), _isStarted(false) {}

Server::~Server() { _stop(); }

void Server::start(const size_t& p_port)
{
	if (_isStarted) throw AlreadyStartedException();
	
	_sock.create();
	if (_sock.isValid() == false) throw StartFailedException("Failed to create socket");

	{
		const int	enable = 1;
		setsockopt(_sock.getFd(), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		setsockopt(_sock.getFd(), SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
	}

	int flags = fcntl(_sock.getFd(), F_GETFL, 0);
	if (flags == -1 || fcntl(_sock.getFd(), F_SETFL, flags | O_NONBLOCK) == -1) {
		_sock.close();
		throw StartFailedException("Failed to set non-blocking mode");
	}

	if (_sock.bind("127.0.0.1", p_port) == false) {
		_sock.close();
		throw StartFailedException("Failed to bind socket");
	}

	if (_sock.listen() == false) {
		_sock.close();
		throw StartFailedException("Failed to listen on socket");
	}

	_isStarted = true;
	_receiver = std::thread(&Server::_receiveMsgs, this);
}

void Server::_stop()
{
	if (!_isStarted) return ;

	_isStarted = false;
	if (_receiver.joinable()) _receiver.join();

	for (auto& [clientID, client] : _clients)
		client.sock.close();

    _clients.clear();
    _msgsToSend.clear();
    _msgs.clear();
    _sock.close();
}

void Server::defineAction(const Message::Type& messageType, const Action& action)
{
	std::lock_guard<std::mutex>	lock(_mtx);
	_actions[messageType] = action;
}

void Server::sendTo(const Message& message, ClientID clientID)
{
	if (!_isStarted) throw NotStartedException();

	std::lock_guard<std::mutex>	lock(_mtx);
	auto it = _msgsToSend.find(clientID);
	if (it == _msgsToSend.end()) throw UnknownClientException();
	it->second.push(message);
}

void
Server::sendToArray(const Message& message, std::vector<ClientID> clientIDs)
{
	if (!_isStarted) throw NotStartedException();

	bool	error = false;
	for (ClientID clientID : clientIDs) {
		try {
			sendTo(message, clientID);
		} catch (const std::exception& e) {
			error = true;
		}
	}

	if (error) throw BatchSendingFailedException();
}

void Server::sendToAll(const Message& message)
{
	if (!_isStarted) throw NotStartedException();

	bool	error = false;
	for (const auto& client : _clients) {
		try {
			sendTo(message, client.first);
		} catch (const std::exception& e) {
			error = true;
		}
	}

	if (error) throw BatchSendingFailedException();
}

void Server::update()
{
	if (!_isStarted) throw NotStartedException();
	if (_shouldEnd) { _stop(); throw NotStartedException(); }

	std::vector<std::pair<ClientID, Message>>	processingList;
	std::unordered_map<Message::Type, Action>	actionsList;

	{
		std::lock_guard<std::mutex>	lock(_mtx);
		processingList.swap(_msgs);
		actionsList = _actions;
	}

	for (auto& [clientID, msg] : processingList) {
		auto	it = actionsList.find(msg.type());
		if (it != actionsList.end() && it->second)
			it->second(clientID, msg);
	}
}

void Server::_acceptConnection()
{
    Socket clientSock = _sock.accept();
    if (!clientSock.isValid()) return;

    int flags = fcntl(clientSock.getFd(), F_GETFL, 0);
    if (flags != -1) fcntl(clientSock.getFd(), F_SETFL, flags | O_NONBLOCK);

    ClientID newClientID = static_cast<ClientID>(clientSock.getFd());
    Client newClient;
    newClient.sock = std::move(clientSock);
    newClient.state = Client::NOSIZE;
    newClient.totalBytes = 0;

    std::lock_guard<std::mutex> lock(_mtx);
    _clients[newClientID] = std::move(newClient);
    _msgsToSend[newClientID] = std::queue<Message>();
}

std::map<Server::ClientID, Server::Client>::iterator 
	Server::_receiveMsg(std::map<ClientID, Client>::iterator it)
{
    Client& client = it->second;

    constexpr size_t MAX_MESSAGE_SIZE = 16 * 1024 * 1024;
    uint32_t rawSize;

    if (client.state == Client::NOSIZE) {
        client.bytesRead = recv(
            client.sock.getFd(),
            reinterpret_cast<char*>(&rawSize) + client.totalBytes,
            sizeof(rawSize) - client.totalBytes,
            0
        );

        if (client.bytesRead <= 0) {
            client.sock.close();
            return _clients.erase(it);
        }

        client.totalBytes += client.bytesRead;

        if (client.totalBytes < sizeof(rawSize))
            return ++it;

        client.size = ntohl(rawSize);

        if (client.size == 0 || client.size > MAX_MESSAGE_SIZE) {
            client.sock.close();
            return _clients.erase(it);
        }

        client.data.resize(client.size, '\0');
        client.totalBytes = 0;
        client.state = Client::SIZE;
    }
    
    if (client.state == Client::SIZE) {
        client.bytesRead = recv(
            client.sock.getFd(),
            client.data.data() + client.totalBytes,
            client.size - client.totalBytes,
            0
        );

        if (client.bytesRead <= 0) {
            client.sock.close();
            return _clients.erase(it);
        }

        client.totalBytes += client.bytesRead;

        if (client.totalBytes < client.size)
            return ++it;

        client.state = Client::MESSAGE;
        client.totalBytes = 0;
    }

    if (client.state == Client::MESSAGE) {
        client.state = Client::NOSIZE;
        Message msg(Message::Type::Undefined);
        try {
            msg.deserialize(client.data);
        } catch (const std::exception&) {
            return ++it;
        }

        _msgs.emplace_back(it->first, std::move(msg));
    }

    return ++it;
}


void Server::_sendMsg(const Message& message, ClientID clientID)
{
	if (!_isStarted) throw NotStartedException();

	auto it = _clients.find(clientID);
	if (it == _clients.end()) throw UnknownClientException();

	std::string	data = message.serialize();
	size_t		size = data.size();

    const Client& client = it->second;
    if (client.sock.isValid() == false) throw UnknownClientException();
    if (client.sock.send(reinterpret_cast<const char*>(&size)) <= 0) throw SendingFailedException();
	if (client.sock.send(data.c_str()) <= 0) throw SendingFailedException();
}

void Server::_receiveMsgs()
{
    fd_set readfds, writefds;
    struct timeval tv;

    while (_isStarted) {
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(_sock.getFd(), &readfds);
        int max_fd = _sock.getFd();

        {
            std::lock_guard<std::mutex> lock(_mtx);
            for (auto& [id, client] : _clients) {
                FD_SET(client.sock.getFd(), &readfds);
                FD_SET(client.sock.getFd(), &writefds);
                if (client.sock.getFd() > max_fd)
                    max_fd = client.sock.getFd();
            }
        }

        tv.tv_sec = 0;
        tv.tv_usec = 100000; // 0.1 sec

        int activity = select(max_fd + 1, &readfds, &writefds, nullptr, &tv);
        if (activity < 0) {
            if (errno == EINTR) continue;
            std::cerr << "[Server] select() failed: " << strerror(errno) << "\n";
            break;
        }

        if (FD_ISSET(_sock.getFd(), &readfds))
            _acceptConnection();

        std::lock_guard<std::mutex> lock(_mtx);
        for (auto it = _clients.begin(); it != _clients.end();) {
            Client& client = it->second;
            if (FD_ISSET(client.sock.getFd(), &readfds)) {
                it = _receiveMsg(it);
            } else {
                ++it;
            }
        }

        for (auto& [clientID, client] : _clients) {
            if (FD_ISSET(client.sock.getFd(), &writefds)) {
                auto msgIt = _msgsToSend.find(clientID);
                if (msgIt != _msgsToSend.end() && !msgIt->second.empty()) {
                    Message msg = msgIt->second.front();
                    try {
                        _sendMsg(msg, clientID);
                    } catch (const std::exception& e) {
                        std::cerr << "[Server] Failed to send to client " << clientID << ": " << e.what() << "\n";
                        client.sock.close();
                    }
                    msgIt->second.pop();
                }
            }
        }
    }
}
