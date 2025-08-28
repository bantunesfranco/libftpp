/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 19:51:44 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/29 01:25:12 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/network/message.hpp"

Message::Message(Type type) : _msgType(type) {}

Message::Type Message::type() { return _msgType; }

std::string Message::serialize() const
{
	std::string	result;
	result.resize(sizeof(_msgType) + _data.size());
	std::memcpy(result.data(), &_msgType, sizeof(_msgType));
	std::memcpy(result.data() + sizeof(_msgType), _data.getData().data(), _data.size());

	return result;
}

void Message::deserialize(const std::string& str)
{
    auto dataSize = sizeof(_msgType);
	if (str.size() < dataSize)
        throw DeserializationFailedException("Failed to receive data");

	std::memcpy(&_msgType, str.data(), dataSize);
	_data.getData().resize(str.size() - dataSize);
	std::memcpy(_data.getData().data(), str.data() + dataSize, str.size() - dataSize);
}
