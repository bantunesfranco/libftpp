/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 19:56:41 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/28 21:47:47 by bfranco       ########   odam.nl         */
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
	public :
		using	Action = std::function<void(Message& msg)>;

	private :
		struct ClientBuf {
			enum State {
				NOSIZE,
				SIZE,
				MESSAGE
			};
			State		state;
			size_t		size;
			std::string	data;
			ssize_t		bytesRead;
			size_t		totalBytes = 0;
		};

		Socket				_sock;
		std::atomic<bool>	_isConnected;
		std::atomic<bool>	_shouldEnd{false};

		std::thread				_receiver;
		std::mutex				_mtx;
		std::vector<Message>	_msgs;
		std::queue<Message>		_msgsToSend;

		std::unordered_map<Message::Type, Action>	_actions;

		void	_receiveMsgs();
		void	_receiveMsg(ClientBuf& clientBuf);
		void	_sendMsg(const Message& message);
	
	public :
		Client();
		~Client();

		void	connect(const std::string& address, const size_t& port);
		void	disconnect();
		void	send(const Message& message);
		void	defineAction(const Message::Type& messageType, const Action& action);
		void	update();

		class AlreadyConnectedException : public std::exception {
			const char*	what() const noexcept { return "Client: Already connected."; };
		};

		class NotConnectedException : public std::exception {
			const char*	what() const noexcept { return "Client: Not connected."; };
		};

		class ConnectionFailedException : public std::runtime_error {
			public :
				ConnectionFailedException(const std::string& msg): runtime_error("Client: " + msg + ".") {}
		};

		class SendingFailedException : public std::runtime_error {
			public :
				SendingFailedException(): runtime_error("Client: Failed to send message.") {}
		};
};

#endif