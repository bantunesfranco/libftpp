/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ivector3.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/23 14:39:50 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/23 14:58:30 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef IVECTOR3_HPP
#define IVECTOR3_HPP

#include <cmath>
#include <stdexcept>

template<typename TType>
struct IVector3 {
	TType x, y, z;

	// Constructors
	IVector3();
	IVector3(TType x, TType y, TType z);
	IVector3(const IVector3& other);
	~IVector3() = default;

	IVector3& operator=(const IVector3& other);

	// Arithmetic
	IVector3 operator+(const IVector3& other) const;
	IVector3 operator-(const IVector3& other) const;
	IVector3 operator*(TType scalar) const;
	IVector3 operator/(TType scalar) const;
	IVector3 operator*(const IVector3& other) const;
	IVector3 operator/(const IVector3& other) const;

	// Compound assignment
	IVector3& operator+=(const IVector3& other);
	IVector3& operator-=(const IVector3& other);
	IVector3& operator*=(TType scalar);
	IVector3& operator/=(TType scalar);
	IVector3& operator*=(const IVector3& other);
	IVector3& operator/=(const IVector3& other);

	// Comparison
	bool operator==(const IVector3& other) const;
	bool operator!=(const IVector3& other) const;

	// Methods
	float length() const;
	float dot(const IVector3& other) const;
	IVector3 cross(const IVector3& other) const;
	IVector3<float> normalized() const;
};

// Constructors
template<typename TType>
IVector3<TType>::IVector3() : x(0), y(0), z(0) {}

template<typename TType>
IVector3<TType>::IVector3(TType x, TType y, TType z) : x(x), y(y), z(z) {}

template<typename TType>
IVector3<TType>::IVector3(const IVector3<TType>& other) : x(other.x), y(other.y), z(other.z) {}

template<typename TType>
IVector3<TType>& IVector3<TType>::operator=(const IVector3<TType>& other) {
	if (this != &other) { x = other.x; y = other.y; z = other.z; }
	return *this;
}

// Arithmetic
template<typename TType>
IVector3<TType> IVector3<TType>::operator+(const IVector3<TType>& other) const {
	return {x + other.x, y + other.y, z + other.z};
}

template<typename TType>
IVector3<TType> IVector3<TType>::operator-(const IVector3<TType>& other) const {
	return {x - other.x, y - other.y, z - other.z};
}

template<typename TType>
IVector3<TType> IVector3<TType>::operator*(TType scalar) const {
	return {x * scalar, y * scalar, z * scalar};
}

template<typename TType>
IVector3<TType> IVector3<TType>::operator/(TType scalar) const {
	if (scalar == 0) throw std::runtime_error("Division by zero");
	return {x / scalar, y / scalar, z / scalar};
}

template<typename TType>
IVector3<TType> IVector3<TType>::operator*(const IVector3<TType>& other) const {
	return {x * other.x, y * other.y, z * other.z};
}

template<typename TType>
IVector3<TType> IVector3<TType>::operator/(const IVector3<TType>& other) const {
	if (other.x == 0 || other.y == 0 || other.z == 0) throw std::runtime_error("Division by zero");
	return {x / other.x, y / other.y, z / other.z};
}

// Compound assignment
template<typename TType>
IVector3<TType>& IVector3<TType>::operator+=(const IVector3<TType>& other) {
	x += other.x; y += other.y; z += other.z;
	return *this;
}

template<typename TType>
IVector3<TType>& IVector3<TType>::operator-=(const IVector3<TType>& other) {
	x -= other.x; y -= other.y; z -= other.z;
	return *this;
}

template<typename TType>
IVector3<TType>& IVector3<TType>::operator*=(TType scalar) {
	x *= scalar; y *= scalar; z *= scalar;
	return *this;
}

template<typename TType>
IVector3<TType>& IVector3<TType>::operator/=(TType scalar) {
	if (scalar == 0) throw std::runtime_error("Division by zero");
	x /= scalar; y /= scalar; z /= scalar;
	return *this;
}

template<typename TType>
IVector3<TType>& IVector3<TType>::operator*=(const IVector3<TType>& other) {
	x *= other.x; y *= other.y; z *= other.z;
	return *this;
}

template<typename TType>
IVector3<TType>& IVector3<TType>::operator/=(const IVector3<TType>& other) {
	if (other.x == 0 || other.y == 0 || other.z == 0) throw std::runtime_error("Division by zero");
	x /= other.x; y /= other.y; z /= other.z;
	return *this;
}

// Comparison
template<typename TType>
bool IVector3<TType>::operator==(const IVector3<TType>& other) const {
	return x == other.x && y == other.y && z == other.z;
}

template<typename TType>
bool IVector3<TType>::operator!=(const IVector3<TType>& other) const {
	return !(*this == other);
}

// Methods
template<typename TType>
float IVector3<TType>::length() const {
	return std::sqrt(x*x + y*y + z*z);
}

template<typename TType>
float IVector3<TType>::dot(const IVector3<TType>& other) const {
	return x * other.x + y * other.y + z * other.z;
}

template<typename TType>
IVector3<TType> IVector3<TType>::cross(const IVector3<TType>& other) const {
	return {
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	};
}

template<typename TType>
IVector3<float> IVector3<TType>::normalized() const {
	float len = length();
	if (len == 0) throw std::runtime_error("Division by zero");
	return {x / len, y / len, z / len};
}

// Friend scalar operator
template<typename TType, typename T>
IVector3<TType> operator*(const T& scalar, const IVector3<TType>& vec) {
	return {scalar * vec.x, scalar * vec.y, scalar * vec.z};
}

#endif
