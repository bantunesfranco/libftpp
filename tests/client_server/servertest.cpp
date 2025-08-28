/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   servertest.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/31 18:31:24 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/29 01:04:48 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <iostream>
#include <atomic>


// int main(int argc, char** argv) {
//     Server server;
//     int serverPort;

//     if (argc != 2) {
//         std::cerr << "[Server] Usage: " << argv[0] << " <port>\n";
//         return 1;
//     }

//     try {
//         serverPort = atoi(argv[1]);
//         server.start(serverPort);
//     } catch (const std::exception& e) {
//         std::cerr << "[Server] Failed to start: " << e.what() << std::endl;
//         return 1;
//     }

//     server.defineAction(Message::Type::Chat, [&](long long& clientId, const Message& msg) {
//         std::cout << "[Server] Chat from client " << clientId << ": " << msg.serialize() << std::endl;

//         Message reply(Message::Type::Chat);
//         reply << "Server echo: " + msg.serialize();
//         server.sendTo(reply, clientId);
//     });

//     server.defineAction(Message::Type::Ping, [&](long long& clientId, const Message& msg) {
//         std::cout << "[Server] Received Ping from client " << clientId << std::endl;

//         Message response(Message::Type::Pong);
//         response << "Pong from server";
//         server.sendTo(response, clientId);
//     });

//     std::cout << "[Server] Running on port " << serverPort << "...\n";

//     while (true) {
//         server.update();
//     }

//     return 0;
// }


#include "core/networking.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

class ChatServer : public Server {
    private:
    std::unordered_map<long long, std::string> _usernames;
    std::unordered_map<std::string, std::vector<long long>> _rooms;
    std::mutex _mtx;

    public:
        void startServer(int port) {
            Server::start(port);

            // Register message handlers
            defineAction(Message::Type::Connect, [&](long long& clientId, Message& msg){
                std::string username;
                msg >> username;
                std::lock_guard<std::mutex> lock(_mtx);
                _usernames[clientId] = username;
                std::cout << "[Server] " << username << " connected (ID " << clientId << ")\n";
            });

            defineAction(Message::Type::Disconnect, [&](long long& clientId, Message& msg){
                std::lock_guard<std::mutex> lock(_mtx);
                std::cout << "[Server] " << _usernames[clientId] << " disconnected\n";
                _usernames.erase(clientId);
                for(auto& [room, clients] : _rooms) {
                    clients.erase(std::remove(clients.begin(), clients.end(), clientId), clients.end());
                }
            });

            defineAction(Message::Type::Chat, [&](long long& clientId, Message& msg){
                std::string room, text;
                msg >> room >> text;
                std::lock_guard<std::mutex> lock(_mtx);

                auto it = _rooms.find(room);
                if(it != _rooms.end()) {
                    Message chat(Message::Type::Chat);
                    chat << _usernames[clientId] + ": " + text;
                    sendToArray(chat, it->second);
                }
            });

            defineAction(Message::Type::DM, [&](long long& clientId, Message& msg){
                std::string targetUser, text;
                msg >> targetUser >> text;

                std::lock_guard<std::mutex> lock(_mtx);
                auto targetIt = std::find_if(_usernames.begin(), _usernames.end(),
                                                [&](auto& pair){ return pair.second == targetUser; });
                if(targetIt != _usernames.end()) {
                    Message dm(Message::Type::DM);
                    dm << "[DM] " + _usernames[clientId] + ": " + text;
                    sendTo(dm, targetIt->first);
                }
            });

            defineAction(Message::Type::Join, [&](long long& clientId, Message& msg){
                std::string room;
                msg >> room;

                std::lock_guard<std::mutex> lock(_mtx);
                auto& clients = _rooms[room];
                if(std::find(clients.begin(), clients.end(), clientId) == clients.end())
                    clients.push_back(clientId);

                Message joinMsg(Message::Type::Chat);
                joinMsg << _usernames[clientId] + " joined room " + room;
                sendToArray(joinMsg, clients);
            });
        }
};


int main() {
    ChatServer server;
    server.startServer(4200);

    std::cout << "[Server] Running...\n";
    while(true) server.update();
}
