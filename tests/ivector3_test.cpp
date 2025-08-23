/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ivector3_test.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/23 15:08:02 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/23 15:13:29 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cmath>
#include <stdexcept>
#include "libftpp.hpp"

void assert(bool condition, const std::string& message) {
	if (!condition) throw std::runtime_error(message);
}

int main() {
	{
		using Vec = IVector3<float>;

		// 1. Constructors
		Vec v0; 
		Vec v1(1.0f, 2.0f, 3.0f); 
		Vec v2(v1); 

		assert(v0.x == 0 && v0.y == 0 && v0.z == 0, "Default constructor failed");
		assert(v1 == Vec(1,2,3), "Parameterized constructor failed");
		assert(v2 == v1, "Copy constructor failed");

		// 2. Arithmetic operators
		Vec add = v1 + v2;
		Vec sub = v1 - v2;
		Vec mulScalar = v1 * 2.0f;
		Vec divScalar = v1 / 2.0f;
		Vec mulComp = v1 * v2;
		Vec divComp = v1 / v2;

		assert(add == Vec(2,4,6), "Addition failed");
		assert(sub == Vec(0,0,0), "Subtraction failed");
		assert(mulScalar == Vec(2,4,6), "Scalar multiplication failed");
		assert(divScalar == Vec(0.5f,1,1.5f), "Scalar division failed");
		assert(mulComp == Vec(1,4,9), "Component-wise multiplication failed");
		assert(divComp == Vec(1,1,1), "Component-wise division failed");

		// 3. Compound assignment operators
		Vec v3(1,1,1);
		v3 += Vec(2,3,4); assert(v3 == Vec(3,4,5), "+= failed");
		v3 -= Vec(1,1,1); assert(v3 == Vec(2,3,4), "-= failed");
		v3 *= 2.0f; assert(v3 == Vec(4,6,8), "*= scalar failed");
		v3 /= 2.0f; assert(v3 == Vec(2,3,4), "/= scalar failed");
		v3 *= Vec(2,3,4); assert(v3 == Vec(4,9,16), "*= component-wise failed");
		v3 /= Vec(2,3,4); assert(v3 == Vec(2,3,4), "/= component-wise failed");

		// 4. Comparison operators
		assert(v1 == v2, "== failed");
		assert(v1 != v3, "!= failed");

		// 5. Methods
		float len = v1.length();
		assert(std::abs(len - std::sqrt(14.0f)) < 1e-6, "length failed");

		Vec norm = v1.normalized();
		float nLen = norm.length();
		assert(std::abs(nLen - 1.0f) < 1e-6, "normalized length failed");

		float dotProd = v1.dot(v2);
		assert(std::abs(dotProd - 14.0f) < 1e-6, "dot failed");

		Vec crossProd = v1.cross(v2);
		assert(crossProd == Vec(0,0,0), "cross failed");
		Vec crossProd2 = v1.cross(Vec(1,1,1));
		assert(crossProd2 == Vec(-1,2,-1), "cross failed");

		// 6. Friend scalar operations
		Vec fmul = 2.0f * v1;
		assert(fmul == Vec(2,4,6), "friend float * failed");
		Vec imul = 2 * v1;
		assert(imul == Vec(2,4,6), "friend int * failed");

		// 7. Division by zero tests
		bool caught = false;
		try {
			Vec test = v1 / 0.0f;
		} catch (const std::runtime_error& e) {
			caught = true;
		}
		assert(caught, "Division by zero (scalar) did not throw");

		caught = false;
		try {
			Vec zeroComp(0,1,1);
			Vec test = v1 / zeroComp;
		} catch (const std::runtime_error& e) {
			caught = true;
		}
		assert(caught, "Division by zero (component-wise) did not throw");
	}
	{
		using Vec = IVector3<int>;

		// 1. Constructors
		Vec v0; 
		Vec v1(1, 2, 3); 
		Vec v2(v1); 

		assert(v0.x == 0 && v0.y == 0 && v0.z == 0, "Default constructor failed");
		assert(v1 == Vec(1,2,3), "Parameterized constructor failed");
		assert(v2 == v1, "Copy constructor failed");

		// 2. Arithmetic operators
		Vec add = v1 + v2;
		Vec sub = v1 - v2;
		Vec mulScalar = v1 * 2;
		Vec divScalar = v1 / 2;
		Vec mulComp = v1 * v2;
		Vec divComp = v1 / v2;

		assert(add == Vec(2,4,6), "Addition failed");
		assert(sub == Vec(0,0,0), "Subtraction failed");
		assert(mulScalar == Vec(2,4,6), "Scalar multiplication failed");
		assert(divScalar == Vec(0.5f,1,1.5f), "Scalar division failed");
		assert(mulComp == Vec(1,4,9), "Component-wise multiplication failed");
		assert(divComp == Vec(1,1,1), "Component-wise division failed");

		// 3. Compound assignment operators
		Vec v3(1,1,1);
		v3 += Vec(2,3,4); assert(v3 == Vec(3,4,5), "+= failed");
		v3 -= Vec(1,1,1); assert(v3 == Vec(2,3,4), "-= failed");
		v3 *= 2; assert(v3 == Vec(4,6,8), "*= scalar failed");
		v3 /= 2; assert(v3 == Vec(2,3,4), "/= scalar failed");
		v3 *= Vec(2,3,4); assert(v3 == Vec(4,9,16), "*= component-wise failed");
		v3 /= Vec(2,3,4); assert(v3 == Vec(2,3,4), "/= component-wise failed");

		// 4. Comparison operators
		assert(v1 == v2, "== failed");
		assert(v1 != v3, "!= failed");

		// 5. Methods
		float len = v1.length();
		assert(std::abs(len - std::sqrt(14)) < 1e-6, "length failed");

		IVector3<float> norm = v1.normalized();
		float nLen = norm.length();
		assert(std::abs(nLen - 1) < 1e-6, "normalized length failed");

		float dotProd = v1.dot(v2);
		assert(std::abs(dotProd - 14) < 1e-6, "dot failed");

		Vec crossProd = v1.cross(v2);
		assert(crossProd == Vec(0,0,0), "cross failed");
		Vec crossProd2 = v1.cross(Vec(1,1,1));
		assert(crossProd2 == Vec(-1,2,-1), "cross failed");

		// 6. Friend scalar operations
		Vec fmul = 2 * v1;
		assert(fmul == Vec(2,4,6), "friend float * failed");
		Vec imul = 2 * v1;
		assert(imul == Vec(2,4,6), "friend int * failed");

		// 7. Division by zero tests
		bool caught = false;
		try {
			Vec test = v1 / 0;
		} catch (const std::runtime_error& e) {
			caught = true;
		}
		assert(caught, "Division by zero (scalar) did not throw");

		caught = false;
		try {
			Vec zeroComp(0,1,1);
			Vec test = v1 / zeroComp;
		} catch (const std::runtime_error& e) {
			caught = true;
		}
		assert(caught, "Division by zero (component-wise) did not throw");
	}
	
	return 0;
}