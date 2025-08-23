/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 22:21:29 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/23 15:44:22 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/networking.hpp"
#include "core/threading.hpp"
#include <cstring>
#include <arpa/inet.h>

Server::Server() : _mutex(), _threads(), _clients(), _receiveBuffers(), _actions(), _messageQueue() {}

Server::~Server() {
    std::lock_guard lock(_mutex);
    for (auto& [_, client] : _clients) {
        client.close();
    }
    _listeningSocket.close();
}

void Server::start(const size_t& port) {
    if (!_listeningSocket.create()) {
        throw std::runtime_error("Failed to create listening socket");
    }

    if (!_listeningSocket.bind("127.0.0.1", static_cast<uint16_t>(port))) {
        throw std::runtime_error("Failed to bind listening socket");
    }

    if (!_listeningSocket.listen(10)) {
        throw std::runtime_error("Failed to listen on socket");
    }

    Thread receiver("receiver", [this]() {
        _receiveFromClients();
    });
    _threads.push_back(std::move(receiver));
    receiver.start();
}

void Server::defineAction(const Message::Type& messageType,
                          const std::function<void(long long& clientID, const Message& msg)>& action) {
    std::lock_guard lock(_mutex);
    _actions[messageType] = action;
}

void Server::sendTo(const Message& message, long long clientID) {
    // std::lock_guard lock(_mutex);
    for (auto& [id, client] : _clients) {
        if (id != clientID) continue;

        std::string payload = message.serialize();
        uint32_t size = htonl(payload.size());

        std::string fullMessage(reinterpret_cast<char*>(&size), sizeof(size));
        fullMessage += payload;

        client.send(fullMessage);
    }
}

void Server::sendToArray(const Message& message, const std::vector<long long>& clientIDs) {
    std::lock_guard lock(_mutex);
    std::string payload = message.serialize();
    uint32_t size = htonl(payload.size());
    
    std::string fullMessage(reinterpret_cast<char*>(&size), sizeof(size));
    fullMessage += payload;
    
    for (auto& [id, client] : _clients) {
        if (std::find(clientIDs.begin(), clientIDs.end(), id) != clientIDs.end())
            client.send(fullMessage);
    }
}

void Server::sendToAll(const Message& message) {
    std::lock_guard lock(_mutex);
    for (const auto& [id, client] : _clients) {
        client.send(message.serialize());
    }
}

void Server::update() {
    std::vector<std::pair<long long, Message>> queueCopy;

    _acceptClients();
    
    {
        std::lock_guard lock(_mutex);
        queueCopy.swap(_messageQueue);
        std::cout << "[Server] Processing " << queueCopy.size() << " messages in the queue." << std::endl;
    }

    for (auto& [clientID, msg] : queueCopy) {
        auto it = _actions.find(static_cast<Message::Type>(msg.type()));
        std::cout << "[Server] Processing message of type " << msg.type() << " from client " << clientID << std::endl;
        if (it != _actions.end()) {
            it->second(clientID, msg);
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Server::_acceptClients() {
    std::cout << "[Server] Waiting for new client connections..." << std::endl;
    Socket clientSocket = _listeningSocket.accept();
    if (!clientSocket.isValid()) return;
    
    std::cout << "[Server] New client connected: " << clientSocket.getFd() << std::endl;
    long long clientID = static_cast<long long>(clientSocket.getFd());

    {
        std::lock_guard lock(_mutex);
        _clients[clientID] = std::move(clientSocket);
    }
}

void Server::_receiveFromClients() {
    constexpr size_t HEADER_SIZE = 4;
    constexpr size_t BUFFER_SIZE = 1024;

    while (true) {
        std::vector<std::pair<long long, Message>> newMessages;

        {
            std::lock_guard<std::mutex> lock(_mutex);
            for (auto it = _clients.begin(); it != _clients.end(); ) {
                long long clientID = it->first;
                Socket& clientSocket = it->second;

                std::string dataChunk = clientSocket.receive(BUFFER_SIZE);

                if (dataChunk.empty()) {
                    _handleClientDisconnect(clientID);
                    it = _clients.erase(it);
                    _receiveBuffers.erase(clientID);
                    continue;
                }

                auto& buffer = _receiveBuffers[clientID];
                buffer += dataChunk;

                while (buffer.size() >= HEADER_SIZE) {
                    uint32_t msgLen;
                    std::memcpy(&msgLen, buffer.data(), HEADER_SIZE);
                    msgLen = ntohl(msgLen);

                    if (msgLen > 1000) {
                        std::cerr << "[Server] Message too large from client " << clientID << std::endl;
                        _handleClientDisconnect(clientID);
                        it = _clients.erase(it);
                        _receiveBuffers.erase(clientID);
                        break;
                    }

                    if (buffer.size() < HEADER_SIZE + msgLen) break;

                    std::string payload = buffer.substr(HEADER_SIZE, msgLen);
                    buffer.erase(0, HEADER_SIZE + msgLen);

                    try {
                        Message msg = Message::deserialize(payload);
                        newMessages.emplace_back(clientID, std::move(msg));
                    } catch (...) {
                        std::cerr << "[Server] Failed to deserialize message from client " << clientID << std::endl;
                        continue;
                    }
                }

                ++it;
            }
        }

        // Add messages to the queue outside of the lock
        for (auto& pair : newMessages) {
            std::lock_guard<std::mutex> lock(_mutex);
            _messageQueue.push_back(std::move(pair));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // prevent busy loop
    }
}

void Server::_handleClientDisconnect(long long clientID) {
    std::lock_guard lock(_mutex);
    
    auto it = _clients.find(clientID);
    if (it != _clients.end()) {
        it->second.close();
        _clients.erase(clientID);
    }
}
