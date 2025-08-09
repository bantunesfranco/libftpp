/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 22:21:29 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/30 23:19:37 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/networking.hpp"
#include "core/threading.hpp"
#include <cstring>
#include <arpa/inet.h>

Server::Server() : _mutex(), _clients(), _receiveBuffers(), _actions(), _messageQueue() {}

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

    std::thread([this]() {
        _acceptClients();
    }).detach();
}

void Server::defineAction(const Message::Type& messageType,
                          const std::function<void(long long& clientID, const Message& msg)>& action) {
    std::lock_guard lock(_mutex);
    _actions[messageType] = action;
}

void Server::sendTo(const Message& message, long long clientID) {
    std::lock_guard lock(_mutex);
    if (auto it = _clients.find(clientID); it != _clients.end()) {
        std::string serialized = message.serialize();
        it->second.send(serialized);
    }
}

void Server::sendToArray(const Message& message, const std::vector<long long>& clientIDs) {
    for (const auto& id : clientIDs) {
        sendTo(message, id);
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

    {
        std::lock_guard lock(_mutex);
        queueCopy.swap(_messageQueue);
    }

    for (auto& [clientID, msg] : queueCopy) {
        auto it = _actions.find(static_cast<Message::Type>(msg.type()));
        if (it != _actions.end()) {
            it->second(clientID, msg);
        }
    }
}

void Server::_acceptClients() {
    while (true) {
        Socket clientSocket = _listeningSocket.accept();
        if (!clientSocket.isValid()) continue;

        long long clientID = static_cast<long long>(clientSocket.getFd());

        {
            std::lock_guard lock(_mutex);
            _clients[clientID] = std::move(clientSocket);
        }

        std::thread([this, clientID]() {
            _receiveFromClient(clientID);
        }).detach();
    }
}

void Server::_receiveFromClient(long long clientID) {
    constexpr size_t HEADER_SIZE = 4;

    while (true) {
        std::string dataChunk;

        {
            std::lock_guard lock(_mutex);
            auto it = _clients.find(clientID);
            if (it == _clients.end()) return;
            dataChunk = it->second.receive();  // could be partial
        }

        if (dataChunk.empty()) {
            _handleClientDisconnect(clientID);
            return;
        }

        std::lock_guard lock(_mutex);
        auto& buffer = _receiveBuffers[clientID];
        buffer += dataChunk;

        // Parse all complete messages
        while (buffer.size() >= HEADER_SIZE) {
            uint32_t msgLen;
            std::memcpy(&msgLen, buffer.data(), HEADER_SIZE);
            msgLen = ntohl(msgLen);

            if (buffer.size() < HEADER_SIZE + msgLen) break; // incomplete

            std::string messagePayload = buffer.substr(HEADER_SIZE, msgLen);
            buffer.erase(0, HEADER_SIZE + msgLen);

            Message msg = Message::deserialize(messagePayload);
            _messageQueue.emplace_back(clientID, std::move(msg));
        }
    }
}

void Server::_handleClientDisconnect(long long clientID) {
    std::lock_guard lock(_mutex);
    _clients.erase(clientID);
}
