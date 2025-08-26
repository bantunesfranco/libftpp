/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   perlin_noise_2D.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 19:49:34 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/25 20:49:06 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERLIN_NOISE_2D_HPP
#define PERLIN_NOISE_2D_HPP

#include "random_2D_coordinate_generator.hpp"
#include "core/mathematics/ivector2.hpp"

class PerlinNoise2D {
	private:
		Random2DCoordinateGenerator _generator;

		using Vec2 = IVector2<float>;

		Vec2 _gradient(int i, int j) const;
		inline static float _fade(float t);
		inline static float _lerp(float a, float b, float t);

	public:
		PerlinNoise2D(long long seed);
		~PerlinNoise2D() = default;

		float seed() const;
		float sample(const float& x, const float& y) const;
		float operator()(const float& x, const float& y) const;
};

#endif