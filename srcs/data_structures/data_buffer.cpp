/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   data_buffer.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 00:33:13 by bfranco       #+#    #+#                 */
/*   Updated: 2024/10/31 21:19:41 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/data_structures.hpp"

DataBuffer::DataBuffer(const DataBuffer& other)
{
	if (this != &other)
		*this = other;
}

DataBuffer& DataBuffer::operator=(const DataBuffer& other)
{
	if (this != &other)
		_buffer = other._buffer;
	return *this;
}

DataBuffer& DataBuffer::operator<<(const std::string &data)
{
	auto	size = data.size();
	if (_buffer.size() + size > _buffer.capacity())
	{
		try {
			_buffer.reserve(_buffer.capacity() + size);
		} catch (const std::bad_alloc &e) {
			std::cerr << "DataBuffer::operator<<: " << e.what() << std::endl;
			return *this;
		}
	}
	auto data_ptr = std::vector<byte>(data.begin(), data.end());
	_buffer.insert(_buffer.end(), data_ptr.begin(), data_ptr.end());
	*this << size;
	return *this;
}

DataBuffer& DataBuffer::operator>>(std::string &data)
{
	std::size_t	size;
	*this >> size;
	if (size > _buffer.size())
		throw std::out_of_range("DataBuffer::operator<<: index out of range");

	auto	end = _buffer.end();
	data.assign(reinterpret_cast<const char*>(_buffer.data() + _buffer.size() - size), size);
	_buffer.erase(end - size, end);
	return *this;
}