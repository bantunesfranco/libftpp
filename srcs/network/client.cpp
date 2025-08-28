/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/28 21:09:23 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/29 00:40:30 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "core/network/client.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>


Client::Client() : _sock(), _isConnected(false) {}

Client::~Client() { disconnect(); }

void Client::connect(const std::string& address, const size_t& port)
{
	if (_isConnected) throw AlreadyConnectedException();

	_sock.create();
	if (!_sock.isValid()) throw ConnectionFailedException("Failed to create socket");

	int flags = fcntl(_sock.getFd(), F_GETFL, 0);
	if (flags == -1 || fcntl(_sock.getFd(), F_SETFL, flags | O_NONBLOCK) == -1) {
		_sock.close();
		throw ConnectionFailedException("Failed to set non-blocking mode");
	}

	sockaddr_in	servAddr{};

	if (address == "localhost") {
		struct addrinfo	hints{}, *res;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		int	status = getaddrinfo("localhost", nullptr, &hints, &res);
		if (status != 0) {
			_sock.close();
			throw ConnectionFailedException("Failed to resolve localhost");
		}

		servAddr.sin_addr = reinterpret_cast<sockaddr_in*>(res->ai_addr)->sin_addr;
		freeaddrinfo(res);
	}

	auto addr = address == "localhost" ? &servAddr : nullptr;
	if (_sock.connect(address, static_cast<uint16_t>(port), addr) == false) {
		if (errno != EINPROGRESS) {
			_sock.close();
			throw ConnectionFailedException("Failed to connect");
		}

		fd_set	writefds;
		FD_ZERO(&writefds);
		FD_SET(_sock.getFd(), &writefds);

		struct timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		int	selectResult = select(_sock.getFd() + 1, nullptr, &writefds, nullptr, &timeout);
		if (selectResult == 0) {
			_sock.close();
			throw ConnectionFailedException("Failed to connect (timeout)");
		} else if (selectResult < 0 || !FD_ISSET(_sock.getFd(), &writefds)) {
			_sock.close();
			throw ConnectionFailedException("Failed to connect (select error)");
		}

		int			error = 0;
		socklen_t	len = sizeof(error);
		if (getsockopt(_sock.getFd(), SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
			_sock.close();
			throw ConnectionFailedException("Failed to connect: " + std::string(strerror(error)));
		}
	}

	_isConnected = true;
	_receiver = std::thread(&Client::_receiveMsgs, this);
}

void Client::disconnect()
{
	if (!_isConnected) return;
	_isConnected = false;

	if (_receiver.joinable()) _receiver.join();
	_sock.close();
}

void Client::send(const Message& message)
{
	if (!_isConnected) throw NotConnectedException();

	std::lock_guard<std::mutex>	lock(_mtx);
	_msgsToSend.push(message);
}

void Client::defineAction(const Message::Type& messageType, const Action& action)
{
	std::lock_guard<std::mutex>	lock(_mtx);
	_actions[messageType] = action;
}

void Client::update()
{
	if (!_isConnected) throw NotConnectedException();
	if (_shouldEnd) { disconnect(); throw NotConnectedException(); }

	std::vector<Message>						processingList;
	std::unordered_map<Message::Type, Action>	actionsList;

	{
		std::lock_guard<std::mutex>	lock(_mtx);
		processingList.swap(_msgs);
		actionsList = _actions;
	}

	for (Message& msg : processingList) {
		auto	it = actionsList.find(msg.type());
		if (it != actionsList.end() && it->second)
			it->second(msg);
	}
}

void Client::_receiveMsg(ClientBuf& clientBuf)
{
	if (clientBuf.state == ClientBuf::NOSIZE) {
		clientBuf.bytesRead = recv(
			_sock.getFd(),
			&clientBuf.size + clientBuf.totalBytes,
			sizeof(clientBuf.size) - clientBuf.totalBytes,
			0
		);

		if (clientBuf.bytesRead <= 0) { _shouldEnd = true; return ; }

		clientBuf.totalBytes += clientBuf.bytesRead;
		if (clientBuf.totalBytes < sizeof(clientBuf.size)) return ;

		clientBuf.state = ClientBuf::SIZE;
		clientBuf.totalBytes = 0;
		clientBuf.data.resize(clientBuf.size, '\0');

	}
	else if (clientBuf.state == ClientBuf::SIZE) {
		clientBuf.bytesRead = recv(
			_sock.getFd(),
			clientBuf.data.data() + clientBuf.totalBytes,
			clientBuf.size - clientBuf.totalBytes,
			0
		);

		if (clientBuf.bytesRead <= 0) { _shouldEnd = true; return ; }

		clientBuf.totalBytes += clientBuf.bytesRead;
		if (clientBuf.totalBytes < clientBuf.size) return ;

		clientBuf.state = ClientBuf::MESSAGE;
		clientBuf.totalBytes = 0;
	}

	if (clientBuf.state == ClientBuf::MESSAGE) {
		clientBuf.state = ClientBuf::NOSIZE;
		Message	msg(Message::Type::Undefined);
		try {
			msg.deserialize(clientBuf.data);
		} catch (const std::exception& e) {
			return ;
		}

		std::lock_guard<std::mutex>	lock(_mtx);
		_msgs.push_back(std::move(msg));
	}
}

void Client::_sendMsg(const Message& message)
{
    if (!_isConnected) throw NotConnectedException();

    std::string data = message.serialize();
    uint32_t len = static_cast<uint32_t>(data.size());
    uint32_t netLen = htonl(len);

    ssize_t sent = ::send(_sock.getFd(), &netLen, sizeof(netLen), 0);
    if (sent != sizeof(netLen)) throw SendingFailedException();

    size_t totalSent = 0;
    while (totalSent < data.size()) {
        sent = ::send(_sock.getFd(), data.data() + totalSent, data.size() - totalSent, 0);
        if (sent <= 0) throw SendingFailedException();
        totalSent += sent;
    }
}


void Client::_receiveMsgs()
{
	ClientBuf	clientBuf;

	fd_set	readfds, writefds;

	while (_isConnected) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(_sock.getFd(), &readfds);
		FD_SET(_sock.getFd(), &writefds);
		int	max_fd = _sock.getFd();

		if (select(max_fd + 1, &readfds, &writefds, nullptr, nullptr) < 0) continue ;

		if (!_shouldEnd && FD_ISSET(_sock.getFd(), &readfds)) { _receiveMsg(clientBuf); }

		if (!_shouldEnd && FD_ISSET(_sock.getFd(), &writefds)) {
			Message	msg(Message::Type::Undefined);
			bool	send = false;
			{
				std::lock_guard<std::mutex>	lock(_mtx);
				if (!_msgsToSend.empty()) {
					msg = _msgsToSend.front();
					_msgsToSend.pop();
					send = true;
				}
			}
			if (send) _sendMsg(msg);
		}
	}
}