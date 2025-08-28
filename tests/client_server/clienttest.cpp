/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   clienttest.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/31 18:31:43 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/29 00:58:12 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

// int main(int argc, char** argv) {
//     Client client;

//     if (argc != 3) {
//         std::cerr << "[Client] Usage: " << argv[0] << " <server_ip> <server_port>\n";
//         return 1;
//     }

//     char* serverIP = argv[1];
//     int serverPort = atoi(argv[2]);

//     try {
//         client.connect(std::string(serverIP), serverPort);
//     } catch (const std::exception& e) {
//         std::cerr << "[Client] Connection error: " << e.what() << std::endl;
//         return 1;
//     }

//     client.defineAction(Message::Type::Chat, [](const Message& msg) {
//         std::cout << "[Client] Received: " << msg.serialize() << std::endl;
//     });

//     client.defineAction(Message::Type::Pong, [](const Message& msg) {
//         std::cout << "[Client] Got PONG: " << msg.serialize()<< std::endl;
//     });

//     std::cout << "[Client] Connected. Type messages or commands:\n" << std::endl;

//     while (true) {
//         std::string line;
//         if (!std::getline(std::cin, line)) break;

//         std::cout << "[Client] Input: " << line << std::endl;
//         if (line.empty()) continue;

//         if (line == "/quit") {
//             break;
//         } else if (line == "/ping") {
//             Message ping(Message::Type::Ping);
//             ping << "Ping from client";
//             client.send(ping);
//         } else {
//             Message chat(Message::Type::Chat);
//             chat << line;
//             client.send(chat);
//         }
        
//         client.update();
//     }

//     std::cout << "Client exited.\n";
//     return 0;
// }

class ChatClient : public Client {
public:
    void connectAndRegister(const std::string& ip, int port, const std::string& username) {
        Client::connect(ip, port);

        Message connectMsg(Message::Type::Connect);
        connectMsg << username;
        send(connectMsg);

        defineAction(Message::Type::Chat, [&](const Message& msg){
            std::cout << msg.serialize() << std::endl;
        });

        defineAction(Message::Type::DM, [&](const Message& msg){
            std::cout << msg.serialize() << std::endl;
        });
    }

    void joinRoom(const std::string& room) {
        Message joinMsg(Message::Type::Join);
        joinMsg << room;
        send(joinMsg);
    }

    void sendChat(const std::string& room, const std::string& text) {
        Message msg(Message::Type::Chat);
        msg << room << text;
        send(msg);
    }

    void sendDM(const std::string& user, const std::string& text) {
        Message msg(Message::Type::DM);
        msg << user << text;
        send(msg);
    }
};

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "[Client] Usage: " << argv[0] << " <username>\n";
        return 1;
    }

    ChatClient client;
    std::string username = std::string(argv[1]);

    client.connectAndRegister("127.0.0.1", 4200, username);
    client.joinRoom("general");

    client.sendChat("general", "Hello everyone!");
    client.sendDM("Bob", "Hey Bob, how are you?");

    while(true) client.update();
}



