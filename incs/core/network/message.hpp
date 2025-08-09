/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 19:32:59 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/31 18:47:10 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <concepts>

class Message {
	public:
		enum class Type : int {
			Undefined = 0,
			Connect,
			Disconnect,
			Chat,
			Ping,
			Pong
		};

	private:
		Type _type = Type::Undefined;
		std::stringstream _stream;

	public:
		Message(int type);
		~Message() = default;
		Message(const Message& other);
		Message& operator=(const Message& other);

		template <typename T>
		friend Message& operator<<(Message& msg, const T& data) {
			msg._stream << data;
			return msg;
		}

		template <typename T>
		friend Message& operator>>(Message& msg, T& data) {
			msg._stream >> data;
			return msg;
		}

		int type() const;
		void setContent(const std::string& msg);
		const std::string content() const ;

		std::string serialize() const;
		static Message deserialize(const std::string& raw);
};

struct MessageTypeHash {
    std::size_t operator()(const Message::Type& t) const noexcept {
        return static_cast<std::size_t>(t);
    }
};

#endif