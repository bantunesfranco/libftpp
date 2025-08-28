/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 19:32:59 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/29 00:51:47 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

# include "core/data_structures/data_buffer.hpp"
# include <stdexcept>


class Message {
	public:
		enum class Type : int {
			Undefined = 0,
			Connect,
			Disconnect,
			Chat,
			Ping,
			Pong,
			DM,
			Join 
		};
	
	private :
		Type		_msgType;
		DataBuffer	_data;

	public :
		Message(Type type);

		Type	type();

		template <typename TType>
		friend Message&	operator<<(Message& msg, const TType& obj);

		template <typename TType>
		friend Message&	operator>>(Message& msg, TType& obj);

		std::string	serialize() const;
		void		deserialize(const std::string& data);

		class DeserializationFailedException : public std::runtime_error {
			public :
				DeserializationFailedException(const std::string& msg) : runtime_error("Message: " + msg + ".") {}
		};
};

template <typename TType>
Message& operator<<(Message& msg, const TType& obj)
{
	msg._data << obj;
	return msg;
}

template <typename TType>
Message& operator>>(Message& msg, TType& obj)
{
	msg._data >> obj;
	return msg;
}

#endif