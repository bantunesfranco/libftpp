/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   servertest.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/31 18:31:24 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/23 15:44:00 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <iostream>
#include <atomic>


int main(int argc, char** argv) {
    Server server;
    int serverPort;

    if (argc != 2) {
        std::cerr << "[Server] Usage: " << argv[0] << " <port>\n";
        return 1;
    }

    try {
        serverPort = atoi(argv[1]);
        server.start(serverPort);
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

    std::cout << "[Server] Running on port " << serverPort << "...\n";

    // Main server loop
    while (true) {
        server.update();
    }

    return 0;
}
