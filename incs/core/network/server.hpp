/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 22:01:22 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/29 00:35:25 by bfranco       ########   odam.nl         */
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
	public :
		using ClientID = long long;
		using Action = std::function<void(ClientID& clientID, Message& msg)>;

		struct Client {
			enum State {
				NOSIZE,
				SIZE,
				MESSAGE
			};
			Socket		sock;
			State		state{State::NOSIZE};
			size_t		size = 0;
			std::string	data;
			ssize_t		bytesRead;
			size_t		totalBytes = 0;
		};
		
	private :
		Socket						_sock;
		std::map<ClientID, Client>	_clients;
		std::atomic<bool>			_isStarted;
		std::atomic<bool>			_shouldEnd{false};

		std::thread									_receiver;
		std::mutex									_mtx;
		std::vector<std::pair<ClientID, Message>>	_msgs;
		std::map<ClientID, std::queue<Message>>		_msgsToSend;

		std::unordered_map<Message::Type, Action>	_actions;

		void									_stop();
		void									_receiveMsgs();
		void									_acceptConnection();
		std::map<ClientID, Client>::iterator	_receiveMsg(std::map<ClientID, Client>::iterator it);
		void									_sendMsg(const Message& message, ClientID clientID);

	public :
		Server();
		~Server();

		void	start(const size_t& p_port);

		void	defineAction(const Message::Type& messageType, const Action& action);
		void	sendTo(const Message& message, ClientID clientID);
		void	sendToArray(const Message& message, std::vector<ClientID> clientIDs);
		void	sendToAll(const Message& message);

		void	update();

		class AlreadyStartedException : public std::exception {
			const char*	what() const noexcept { return "Server: Already started."; };
		};

		class NotStartedException : public std::exception {
			const char*	what() const noexcept { return "Server: Not started."; };
		};

		class StartFailedException : public std::runtime_error {
			public :
				StartFailedException(const std::string& msg) : runtime_error("Server: " + msg + ".") {}
		};

		class UnknownClientException : public std::runtime_error {
			public :
				UnknownClientException() : runtime_error("Server: Unknown client.") {}
		};

		class SendingFailedException : public std::runtime_error {
			public :
				SendingFailedException() : runtime_error("Server: Failed to send message.") {}
		};

		class BatchSendingFailedException : public std::runtime_error {
			public :
				BatchSendingFailedException() : runtime_error("Server: Failed to send at least 1 message.") {}
		};
};



#endif