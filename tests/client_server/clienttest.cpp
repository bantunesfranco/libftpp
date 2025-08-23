/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   clienttest.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/31 18:31:43 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/23 15:35:06 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

int main(int argc, char** argv) {
    Client client;

    if (argc != 3) {
        std::cerr << "[Client] Usage: " << argv[0] << " <server_ip> <server_port>\n";
        return 1;
    }

    char* serverIP = argv[1];
    int serverPort = atoi(argv[2]);

    if (!client.connect(std::string(serverIP), serverPort)) {
        std::cerr << "[Client] Failed to connect to server.\n";
        return 1;
    }

    // Capture client by value in the lambdas to ensure they remain valid
    client.defineAction(Message::Type::Chat, [](const Message& msg) {
        std::cout << "[Client] Received: " << msg.content() << std::endl;
    });

    client.defineAction(Message::Type::Pong, [](const Message& msg) {
        std::cout << "[Client] Got PONG: " << msg.content() << std::endl;
    });

    std::cout << "[Client] Connected. Type messages or commands:\n" << std::endl;

    while (true) {
        std::string line;
        if (!std::getline(std::cin, line)) break;

        std::cout << "[Client] Input: " << line << std::endl;
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

        client.receive();
        client.update();
    }

    client.disconnect();
    std::cout << "Client exited.\n";
    return 0;
}
