/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 19:51:44 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/31 18:43:33 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/network/message.hpp"

Message::Message(int type) : _type(static_cast<Type>(type)) {}

Message::Message(const Message& other) {
    *this = other;
}

Message& Message::operator=(const Message& other) {
    if (this != &other)
    {
        _type = other._type;
        std::stringstream tmp(other._stream.str());
        _stream.swap(tmp);
    }
    return *this;
}

int Message::type() const {
    return static_cast<int>(_type);
}

void Message::setContent(const std::string& msg) {
    std::stringstream tmp(msg);
    _stream.swap(tmp);
}

const std::string Message::content() const {
    const std::string str = _stream.str();
    return str;
}

std::string Message::serialize() const {
    std::ostringstream out;
    out << static_cast<int>(_type) << '\n' << _stream.str();
    return out.str();
}

Message Message::deserialize(const std::string& raw) {
    std::istringstream in(raw);
    int typeInt;
    in >> typeInt;
    in.ignore(); // Skip newline

    std::string content((std::istreambuf_iterator<char>(in)), {});
    Message msg(typeInt);
    msg._stream.str(content);
    return msg;
}

