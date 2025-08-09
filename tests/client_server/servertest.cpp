/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   servertest.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/31 18:31:24 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/31 18:55:27 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <iostream>
#include <atomic>

constexpr uint16_t SERVER_PORT = 5555;

int main() {
    Server server;

    try {
        server.start(SERVER_PORT);
    } catch (const std::exception& e) {
        std::cerr << "[Server] Failed to start: " << e.what() << std::endl;
        return 1;
    }

    server.defineAction(Message::Type::Chat, [&](long long& clientId, const Message& msg) {
        std::cout << "[Server] Chat from client " << clientId << ": " << msg.content() << std::endl;

        Message reply(static_cast<int>(Message::Type::Chat));
        reply.setContent("Server echo: " + msg.content());
        server.sendTo(reply, clientId);
    });

    server.defineAction(Message::Type::Ping, [&](long long& clientId, const Message& msg) {
        std::cout << "[Server] Received Ping from client " << clientId << std::endl;

        Message response(static_cast<int>(Message::Type::Pong));
        response.setContent("Pong!");
        server.sendTo(response, clientId);
    });

    std::cout << "[Server] Running on port " << SERVER_PORT << "...\n";

    // Main server loop
    while (true) {
        server.update();
    }

    return 0;
}
