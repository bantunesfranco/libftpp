/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   random_2D_coordinate_generator.cpp                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 17:41:06 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/25 19:13:37 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/mathematics/random_2D_coordinate_generator.hpp"
#include <random>

// Xoroshiro128+ (https://en.wikipedia.org/wiki/Xorshift#xoroshiro)

static inline uint64_t splitmix64(uint64_t value) {
	uint64_t result = (value += 0x9E3779B97F4A7C15ULL);
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9ULL;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EBULL;
	return result ^ (result >> 31);
}

Random2DCoordinateGenerator::Random2DCoordinateGenerator(long long seed) : _seed(seed) {}

long long Random2DCoordinateGenerator::seed() const { return _seed; }

long long Random2DCoordinateGenerator::operator()(const long long& x, const long long& y) const {

    uint64_t state = static_cast<uint64_t>(_seed);
    state ^= static_cast<uint64_t>(x) + 0x9E3779B97F4A7C15ULL + (state << 6) + (state >> 2);
    state ^= static_cast<uint64_t>(y) + 0x9E3779B97F4A7C15ULL + (state << 6) + (state >> 2);

    return static_cast<long long>(splitmix64(state));
}