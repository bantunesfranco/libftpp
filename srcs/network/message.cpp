/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 19:51:44 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/17 12:36:27 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/network/message.hpp"
#include "core/network/message.hpp"
#include <limits>


Message::Message(int type) : _type(static_cast<Type>(type)) {}

Message::Message(const Message& other) {
    *this = other;
}

Message& Message::operator=(const Message& other) {
    if (this != &other) {
        _type = other._type;
        _stream.str(other._stream.str());
        _stream.clear(); // reset error flags
    }
    return *this;
}

int Message::type() const {
    return static_cast<int>(_type);
}

void Message::setContent(const std::string& msg) {
    _stream.str(msg);
    _stream.clear();
}

const std::string Message::content() const {
    return _stream.str();
}

std::string Message::serialize() const {
    std::ostringstream out;
    out << static_cast<int>(_type) << '\n' << _stream.str();
    return out.str();
}

Message Message::deserialize(const std::string& raw) {
    std::istringstream in(raw);
    int typeInt = 0;
    in >> typeInt;
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // safer newline skip

    std::string content((std::istreambuf_iterator<char>(in)), {});
    Message msg(typeInt);
    msg.setContent(content);
    return msg;
}
