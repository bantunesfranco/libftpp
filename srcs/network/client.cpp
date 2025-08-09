/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 20:13:11 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/30 23:33:40 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/networking.hpp"
#include <arpa/inet.h>
#include <cstring>

Client::Client() = default;

bool Client::connect(const std::string& address, const size_t& port) {
    _socket.create();
    return _socket.connect(address, static_cast<uint16_t>(port));
}

void Client::disconnect() {
    _socket.close();
}

void Client::defineAction(Message::Type messageType,
                          const std::function<void(const Message&)>& action) {
    std::lock_guard<std::mutex> lock(_mutex);
    _actions[messageType] = action;
}

void Client::send(const Message& message) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_socket.isValid()) return;

    std::string payload = message.serialize();
    uint32_t size = htonl(static_cast<uint32_t>(payload.size()));

    std::string fullMessage(reinterpret_cast<const char*>(&size), sizeof(size));
    fullMessage += payload;

    _socket.send(fullMessage);
}

void Client::update() {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!_socket.isValid()) return;

    std::string chunk = _socket.receive(1024);
    _receiveBuffer += chunk;

    constexpr size_t HEADER_SIZE = 4;

    while (_receiveBuffer.size() >= HEADER_SIZE) {
        uint32_t msgLen;
        std::memcpy(&msgLen, _receiveBuffer.data(), HEADER_SIZE);
        msgLen = ntohl(msgLen);

        if (_receiveBuffer.size() < HEADER_SIZE + msgLen) break;

        std::string messagePayload = _receiveBuffer.substr(HEADER_SIZE, msgLen);
        _receiveBuffer.erase(0, HEADER_SIZE + msgLen);

        Message msg = Message::deserialize(messagePayload);
        _messageQueue.push_back(msg);
    }

    while (!_messageQueue.empty()) {
        Message msg = _messageQueue.front();
        _messageQueue.pop_front();

        auto it = _actions.find(static_cast<Message::Type>(msg.type()));
        if (it != _actions.end()) {
            it->second(msg);
        }
    }
}
