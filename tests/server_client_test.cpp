/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_client_test.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 23:35:01 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/31 18:25:08 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/threading.hpp"
#include "core/networking.hpp"
#include <iostream>
#include <thread>
#include <cassert>
#include <chrono>
#include <atomic>
#include <string_view>

constexpr uint16_t TEST_PORT = 5555;
constexpr std::string_view TEST_IP = "127.0.0.1";

std::atomic<bool> running{true};

void run_server() {
    Server server;
    server.start(TEST_PORT);

    server.defineAction(Message::Type::Chat, [&](long long& clientId, const Message& msg) {
        std::cout << "[Server] Chat from client " << clientId << ": " << msg.content() << std::endl;

        // Echo back
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

    while (running) {
        server.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    std::thread serverThread(run_server);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Client client;
    if (!client.connect(std::string(TEST_IP), TEST_PORT)) {
        std::cerr << "Failed to connect to server.\n";
        return 1;
    }

    client.defineAction(Message::Type::Chat, [&](const Message& msg) {
        std::cout << "[Client] Received: " << msg.content() << std::endl;
    });

    client.defineAction(Message::Type::Pong, [&](const Message& msg) {
        std::cout << "[Client] Got PONG: " << msg.content() << std::endl;
    });

    std::thread inputThread([&]() {
        std::string line;
        while (running && std::getline(std::cin, line)) {
            if (line.empty()) continue;

            if (line == "/quit") {
                running = false;
                break;
            } else if (line == "/ping") {
                Message ping(static_cast<int>(Message::Type::Ping));
                ping.setContent("Ping from client");
                client.send(ping);
            } else {
                Message chat(static_cast<int>(Message::Type::Chat));
                chat.setContent(line);
                client.send(chat);
            }
        }
    });

    // Client update loop
    while (running) {
        client.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    inputThread.join();
    client.disconnect();
    serverThread.join();

    std::cout << "Client exited.\n";
    return 0;
}
