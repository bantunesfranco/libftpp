/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 22:01:22 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/23 15:43:52 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include "core/networking.hpp"
#include "core/threading.hpp"

class Server {
	private:
		Socket _listeningSocket;
		std::mutex _mutex;
		std::vector<Thread> _threads;

		std::unordered_map<long long, Socket> _clients;
		std::unordered_map<long long, std::string> _receiveBuffers;
		std::unordered_map<Message::Type, std::function<void(long long&, const Message&)>, MessageTypeHash> _actions;

		std::vector<std::pair<long long, Message>> _messageQueue;

		void _acceptClients();
		void _receiveFromClients();
		void _handleClientDisconnect(long long clientID);
	
	public:
		Server();
		~Server();
		
		void start(const size_t& port);
		void defineAction(const Message::Type& messageType, const std::function<void(long long& clientID, const Message& msg)>& action);
		void sendTo(const Message& message, long long clientID);
		void sendToArray(const Message& message, const std::vector<long long>& clientIDs);
		void sendToAll(const Message& message);
		void update();
};


#endif