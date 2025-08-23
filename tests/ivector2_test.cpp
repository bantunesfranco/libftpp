/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ivector2_test.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/23 14:03:00 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/23 15:14:47 by bfranco       ########   odam.nl         */
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
        using Vec = IVector2<float>;

        // 1. Constructors
        Vec v0; 
        Vec v1(3.0f, 4.0f); 
        Vec v2(v1); 

        assert(v0.x == 0 && v0.y == 0, "Default constructor failed");
        assert(v1 == Vec(3,4), "Parameterized constructor failed");
        assert(v2 == v1, "Copy constructor failed");

        // 2. Arithmetic operators
        Vec add = v1 + v2;
        Vec sub = v1 - v2;
        Vec mulScalar = v1 * 2.0f;
        Vec divScalar = v1 / 2.0f;
        Vec mulComp = v1 * v2;
        Vec divComp = v1 / v2;

        assert(add == Vec(6,8), "Addition failed");
        assert(sub == Vec(0,0), "Subtraction failed");
        assert(mulScalar == Vec(6,8), "Scalar multiplication failed");
        assert(divScalar == Vec(1.5f,2), "Scalar division failed");
        assert(mulComp == Vec(9,16), "Component-wise multiplication failed");
        assert(divComp == Vec(1,1), "Component-wise division failed");

        // 3. Compound assignment operators
        Vec v3(1,1);
        v3 += Vec(2,3); assert(v3 == Vec(3,4), "+= failed");
        v3 -= Vec(1,1); assert(v3 == Vec(2,3), "-= failed");
        v3 *= 2.0f; assert(v3 == Vec(4,6), "*= scalar failed");
        v3 /= 2.0f; assert(v3 == Vec(2,3), "/= scalar failed");
        v3 *= Vec(2,3); assert(v3 == Vec(4,9), "*= component-wise failed");
        v3 /= Vec(2,3); assert(v3 == Vec(2,3), "/= component-wise failed");

        // 4. Comparison operators
        assert(v1 == v2, "== failed");
        assert(v1 != v3, "!= failed");

        // 5. Methods
        float len = v1.length();
        assert(std::abs(len - 5.0f) < 1e-6, "length failed");

        Vec norm = v1.normalized();
        assert(std::abs(norm.x - 0.6f) < 1e-6 && std::abs(norm.y - 0.8f) < 1e-6, "normalized failed");

        float dotProd = v1.dot(v2);
        assert(std::abs(dotProd - 25.0f) < 1e-6, "dot failed");

        Vec crossProd = v1.cross();
        assert(crossProd == Vec(-4,3), "cross failed");

        // 6. Friend scalar operations
        Vec fmul = 2.0f * v1;
        assert(fmul == Vec(6,8), "friend float * failed");
        Vec imul = 2 * v1;
        assert(imul == Vec(6,8), "friend int * failed");

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
            Vec zeroComp(0,1);
            Vec test = v1 / zeroComp;
        } catch (const std::runtime_error& e) {
            caught = true;
        }
        assert(caught, "Division by zero (component-wise) did not throw");
    }
    { using Vec = IVector2<int>;

        // 1. Constructors
        Vec v0; 
        Vec v1(3, 4); 
        Vec v2(v1); 

        assert(v0.x == 0 && v0.y == 0, "Default constructor failed");
        assert(v1 == Vec(3,4), "Parameterized constructor failed");
        assert(v2 == v1, "Copy constructor failed");

        // 2. Arithmetic operators
        Vec add = v1 + v2;
        Vec sub = v1 - v2;
        Vec mulScalar = v1 * 2;
        Vec divScalar = v1 / 2;
        Vec mulComp = v1 * v2;
        Vec divComp = v1 / v2;

        assert(add == Vec(6,8), "Addition failed");
        assert(sub == Vec(0,0), "Subtraction failed");
        assert(mulScalar == Vec(6,8), "Scalar multiplication failed");
        assert(divScalar == Vec(1.5f,2), "Scalar division failed");
        assert(mulComp == Vec(9,16), "Component-wise multiplication failed");
        assert(divComp == Vec(1,1), "Component-wise division failed");

        // 3. Compound assignment operators
        Vec v3(1,1);
        v3 += Vec(2,3); assert(v3 == Vec(3,4), "+= failed");
        v3 -= Vec(1,1); assert(v3 == Vec(2,3), "-= failed");
        v3 *= 2; assert(v3 == Vec(4,6), "*= scalar failed");
        v3 /= 2; assert(v3 == Vec(2,3), "/= scalar failed");
        v3 *= Vec(2,3); assert(v3 == Vec(4,9), "*= component-wise failed");
        v3 /= Vec(2,3); assert(v3 == Vec(2,3), "/= component-wise failed");

        // 4. Comparison operators
        assert(v1 == v2, "== failed");
        assert(v1 != v3, "!= failed");

        // 5. Methods
        float len = v1.length();
        assert(std::abs(len - 5) < 1e-6, "length failed");

        IVector2<float> norm = v1.normalized();
        assert(std::abs(norm.x - 0.6f) < 1e-6 && std::abs(norm.y - 0.8f) < 1e-6, "normalized failed");

        float dotProd = v1.dot(v2);
        assert(std::abs(dotProd - 25) < 1e-6, "dot failed");

        Vec crossProd = v1.cross();
        assert(crossProd == Vec(-4,3), "cross failed");

        // 6. Friend scalar operations
        Vec fmul = 2 * v1;
        assert(fmul == Vec(6,8), "friend float * failed");
        Vec imul = 2 * v1;
        assert(imul == Vec(6,8), "friend int * failed");

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
            Vec zeroComp(0,1);
            Vec test = v1 / zeroComp;
        } catch (const std::runtime_error& e) {
            caught = true;
        }
        assert(caught, "Division by zero (component-wise) did not throw");
    }
    return 0;
}
