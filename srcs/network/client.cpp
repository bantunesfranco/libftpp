/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 20:13:11 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/23 15:41:23 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/networking.hpp"
#include "core/threading.hpp"
#include <arpa/inet.h>
#include <cstring>

// Client::Client(): _mutex(), _actions(), _messageQueue(), _receiveBuffer(),
//                 _receiver("receiver", [this] { this->_receive(); }) {}
Client::Client(): _mutex(), _actions(), _messageQueue(), _receiveBuffer() {};

bool Client::connect(const std::string& address, const size_t& port) {
    _socket.create();

    std::lock_guard<std::mutex> lock(_mutex);
    bool connected = _socket.connect(address, static_cast<uint16_t>(port));
    if (!connected) return false;

    // _receiver.start();

    return connected;
}

void Client::disconnect() {
    _socket.close();
}

void Client::receive() {
    constexpr size_t HEADER_SIZE = 4;
    constexpr size_t BUFFER_SIZE = 1024;

    // while (true) {
        std::string chunk = _socket.receive(BUFFER_SIZE);
        if (chunk.empty()) {
            return;
            // std::this_thread::sleep_for(std::chrono::milliseconds(10));
            // continue;
        }

        // {
            // std::lock_guard<std::mutex> lock(_mutex);
            if (!_socket.isValid()) return;
            _receiveBuffer += chunk;
        // }

        while (true) {
            std::string payload;
            {
                // std::lock_guard<std::mutex> lock(_mutex);
                if (_receiveBuffer.size() < HEADER_SIZE) break;

                uint32_t msgLen;
                std::memcpy(&msgLen, _receiveBuffer.data(), HEADER_SIZE);
                msgLen = ntohl(msgLen);

                if (_receiveBuffer.size() < HEADER_SIZE + msgLen) break;

                payload = _receiveBuffer.substr(HEADER_SIZE, msgLen);
                _receiveBuffer.erase(0, HEADER_SIZE + msgLen);
            }


            Message msg = Message::deserialize(payload);

            // {
            //     std::lock_guard<std::mutex> lock(_mutex);
                _messageQueue.push_back(msg);
            // }
        }
    // }
}

void Client::defineAction(Message::Type messageType, const std::function<void(const Message&)>& action) {
    // std::lock_guard<std::mutex> lock(_mutex);
    _actions[messageType] = action;
}

void Client::send(const Message& message) {
    // std::lock_guard<std::mutex> lock(_mutex);
    if (!_socket.isValid()) return;

    std::string payload = message.serialize();
    uint32_t size = htonl(static_cast<uint32_t>(payload.size()));

    std::string fullMessage;
    fullMessage.reserve(sizeof(size) + payload.size());
    fullMessage.append(reinterpret_cast<const char*>(&size), sizeof(size));
    fullMessage.append(payload);

    std::cout << "[Client] Sending message of type " << message.type()
              << ": " << message.content() << std::endl;

    size_t totalSent = 0;
    while (totalSent < fullMessage.size()) {
        size_t sent = _socket.send(
            std::string_view(fullMessage).substr(totalSent)
        );
        if (sent <= 0) {
            std::cerr << "[Client] Failed to send message." << std::endl;
            break;
        }
        totalSent += sent;
    }
}


void Client::update() {
    std::vector<Message> tmpMessages;

    // {
    //     std::lock_guard<std::mutex> lock(_mutex);
        tmpMessages.swap(_messageQueue);
    // }

    for (const auto& msg : _messageQueue) {
        // std::lock_guard<std::mutex> lock(_mutex);
        auto it = _actions.find(static_cast<Message::Type>(msg.type()));
        if (it != _actions.end()) {
            it->second(msg);
        }
    }
}
