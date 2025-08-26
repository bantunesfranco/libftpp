/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ivector2.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/23 12:56:10 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/24 20:06:24 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef	IVECTOR2_HPP
#define IVECTOR2_HPP

#include <cmath>
#include <stdexcept>

template<typename TType>
struct IVector2
{
	public:
		TType x, y;

		IVector2();
		IVector2(const TType x, const TType y);
		IVector2(const IVector2& other);
		~IVector2();
		
		IVector2& operator=(const IVector2& other);

		// Arithmetic
		IVector2 operator+(const IVector2& other) const;
		IVector2 operator-(const IVector2& other) const;
		IVector2 operator*(const TType& scalar) const;
		IVector2 operator/(const TType& scalar) const;
		IVector2 operator*(const IVector2& other) const;
		IVector2 operator/(const IVector2& other) const;
	
		// Compound assignment
		IVector2& operator+=(const IVector2& other);
		IVector2& operator-=(const IVector2& other);
		IVector2& operator*=(const TType& scalar);
		IVector2& operator/=(const TType& scalar);
		IVector2& operator*=(const IVector2& other);
		IVector2& operator/=(const IVector2& other);
		
		// Comparison
		bool operator==(const IVector2& other) const;
		bool operator!=(const IVector2& other) const;

		// Methods
		float length() const;
		float dot(const IVector2<TType>& other) const;
		IVector2<float> normalized() const;
		IVector2 cross() const;
};

// Constructors
template<typename TType>
IVector2<TType>::IVector2() : x(0), y(0) {}

template<typename TType>
IVector2<TType>::IVector2(TType x, TType y) : x(x), y(y) {}

template<typename TType>
IVector2<TType>::~IVector2() {}

template<typename TType>
IVector2<TType>::IVector2(const IVector2<TType>& other) : x(other.x), y(other.y) {}

template<typename TType>
IVector2<TType>& IVector2<TType>::operator=(const IVector2<TType>& other) {
	if (this != &other) { x = other.x; y = other.y; }
	return *this;
}

// Arithmetic
template<typename TType>
IVector2<TType> IVector2<TType>::operator+(const IVector2<TType>& other) const {
	return { x + other.x, y + other.y };
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator-(const IVector2<TType>& other) const {
	return { x - other.x, y - other.y };
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator*(const TType& scalar) const {
	return { x * scalar, y * scalar };
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator/(const TType& scalar) const {
	if (scalar == 0) throw std::runtime_error("Division by zero");
	return { x / scalar, y / scalar };
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator*(const IVector2<TType>& other) const {
	return { x * other.x, y * other.y };
}

template<typename TType>
IVector2<TType> IVector2<TType>::operator/(const IVector2<TType>& other) const {
	if (other.x == 0 || other.y == 0) throw std::runtime_error("Division by zero");
	return { x / other.x, y / other.y };
}

// Compound assignment
template<typename TType>
IVector2<TType>& IVector2<TType>::operator+=(const IVector2<TType>& other) {
	x += other.x;
	y += other.y;
	return *this;
}

template<typename TType>
IVector2<TType>& IVector2<TType>::operator-=(const IVector2<TType>& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

template<typename TType>
IVector2<TType>& IVector2<TType>::operator*=(const TType& scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

template<typename TType>
IVector2<TType>& IVector2<TType>::operator/=(const TType& scalar) {
	if (scalar == 0) throw std::runtime_error("Division by zero");
	x /= scalar;
	y /= scalar;
	return *this;
}

template<typename TType>
IVector2<TType>& IVector2<TType>::operator*=(const IVector2<TType>& other) {
	x *= other.x;
	y *= other.y;
	return *this;
}

template<typename TType>
IVector2<TType>& IVector2<TType>::operator/=(const IVector2<TType>& other) {
	if (other.x == 0 || other.y == 0) throw std::runtime_error("Division by zero");
	x /= other.x;
	y /= other.y;
	return *this;
}

// Comparison
template<typename TType>
bool IVector2<TType>::operator==(const IVector2<TType>& other) const {
	return x == other.x && y == other.y;
}

template<typename TType>
bool IVector2<TType>::operator!=(const IVector2<TType>& other) const {
	return !(*this == other);
}

// Methods
template<typename TType>
float IVector2<TType>::dot(const IVector2<TType>& other) const {
	return x * other.x + y * other.y;
}

template<typename TType>
IVector2<TType> IVector2<TType>::cross() const {
	return { -y, x };
}

template<typename TType>
float IVector2<TType>::length() const {
	return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

template<typename TType>
IVector2<float> IVector2<TType>::normalized() const {
	float len = length();
	if (len == 0) throw std::runtime_error("Division by zero");
	return { static_cast<float>(x) / len, static_cast<float>(y) / len };
}

// Friend scalar operator
template<typename TType, typename T>
IVector2<TType> operator*(const T& scalar, const IVector2<TType>& vec) {
	return { scalar * vec.x, scalar * vec.y };
}

#endif