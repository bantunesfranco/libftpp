/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   data_buffer.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/26 20:30:13 by bfranco       #+#    #+#                 */
/*   Updated: 2024/10/31 21:05:20 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#include <vector>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <iostream>

typedef uint8_t byte;

class DataBuffer
{
	private:
		std::vector<byte> _buffer;

	public:
		DataBuffer() = default;
		~DataBuffer() = default;
		DataBuffer(const DataBuffer &other);
		DataBuffer &operator=(const DataBuffer &other);

		DataBuffer &operator<<(const std::string &data);
		DataBuffer &operator>>(std::string &data);
	
		template<typename T>
		DataBuffer &operator<<(const T &data);
		template<typename T>
		DataBuffer &operator>>(T &data);
};

template<typename T>
DataBuffer& DataBuffer::operator<<(const T &data)
{
	auto	size = sizeof(T);
	if (_buffer.size() + size > _buffer.capacity())
	{
		try {
			_buffer.reserve(_buffer.capacity() + size);
		} catch (const std::bad_alloc &e) {
			std::cerr << "DataBuffer::operator<<: " << e.what() << std::endl;
			return *this;
		}
	}
	auto data_ptr = reinterpret_cast<const byte*>(&data);
	_buffer.insert(_buffer.end(), data_ptr, data_ptr + size);
	return *this;
}

template<typename T>
DataBuffer& DataBuffer::operator>>(T &data)
{
	auto	size = sizeof(T);
	if (size > _buffer.size())
		throw std::out_of_range("DataBuffer::operator<<: index out of range");

	auto	begin = _buffer.end() - size;
	std::copy(begin, begin + size, reinterpret_cast<byte*>(&data));
	_buffer.erase(begin, begin + size);
	return *this;
}

#endif