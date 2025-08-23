/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 19:56:41 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/23 15:39:27 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <vector>

#include "core/networking.hpp"
#include "core/threading.hpp"

class Client {
	private:
		Socket _socket;
		mutable std::mutex _mutex;
		std::unordered_map<Message::Type, std::function<void(const Message&)>, MessageTypeHash> _actions;
		// ThreadSafeQueue<Message> _messageQueue;
		std::vector<Message> _messageQueue;
		std::string _receiveBuffer;
		// Thread _receiver;

		
	public:
		Client();
		
		bool connect(const std::string& address, const size_t& port);
		void disconnect();
		void defineAction(Message::Type messageType, const std::function<void(const Message&)>& action);
		void receive();
		void send(const Message& message);
		void update();
};

#endif