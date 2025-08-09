/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   clienttest.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/31 18:31:43 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/31 18:52:54 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

constexpr uint16_t SERVER_PORT = 5555;
constexpr std::string_view SERVER_IP = "127.0.0.1";

int main() {
    Client client;

    if (!client.connect(std::string(SERVER_IP), SERVER_PORT)) {
        std::cerr << "[Client] Failed to connect to server.\n";
        return 1;
    }

    client.defineAction(Message::Type::Chat, [&](const Message& msg) {
        std::cout << "[Client] Received: " << msg.content() << std::endl;
    });

    client.defineAction(Message::Type::Pong, [&](const Message& msg) {
        std::cout << "[Client] Got PONG: " << msg.content() << std::endl;
    });

    std::cout << "[Client] Connected. Type messages or commands:\n";

    while (true) {
        std::string line;
        if (!std::getline(std::cin, line)) break;

        if (line.empty()) continue;

        if (line == "/quit") {
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

        client.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    client.disconnect();
    std::cout << "Client exited.\n";
    return 0;
}
