/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   perlin_noise_2D.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 19:54:24 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/25 20:48:41 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/mathematics/perlin_noise_2D.hpp"
#include <cmath>

using Vec2 = IVector2<float>;

PerlinNoise2D::PerlinNoise2D(long long seed) : _generator(seed) {}

float PerlinNoise2D::seed() const { return _generator.seed(); }

float PerlinNoise2D::operator()(const float& x, const float& y) const {
	return sample(x, y);
}

// Smooth interpolation curve
inline float PerlinNoise2D::_fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10); 
}

// Linear interpolation
inline float PerlinNoise2D::_lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Pseudo-random gradient at integer grid point (i, j)
Vec2 PerlinNoise2D::_gradient(int i, int j) const {
    // Hash function -> repeatable pseudo-random angle
    long long h = _generator(i, j);
    float angle = (1.0f - ((h * (h * h * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f) * M_PI;
    return { cosf(angle), sinf(angle) };
}

float PerlinNoise2D::sample(const float& x, const float& y) const {
	// Lattice cell coordinates
    int i = floor(x);
    int j = floor(y);

    // Local position in cell
    float u = x - i;
    float v = y - j;

    // Get gradients for 4 corners
    Vec2 g00 = _gradient(i, j);
    Vec2 g10 = _gradient(i + 1, j);
    Vec2 g01 = _gradient(i, j + 1);
    Vec2 g11 = _gradient(i + 1, j + 1);

    // Displacement vectors from corners to point
    Vec2 d00(u, v);
    Vec2 d10(u - 1, v);
    Vec2 d01(u, v - 1);
    Vec2 d11(u - 1, v - 1);

    // Dot products
    float d_00 = g00.dot(d00);
    float d_10 = g10.dot(d10);
    float d_01 = g01.dot(d01);
    float d_11 = g11.dot(d11);

    // Fade curves for u,v
    float sx = _fade(u);
    float sy = _fade(v);

    // Interpolate in x, then y
    float ix0 = _lerp(d_00, d_10, sx);
    float ix1 = _lerp(d_01, d_11, sx);
    float value = _lerp(ix0, ix1, sy);

    return value;
}