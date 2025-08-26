/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   random_2D_coordinate_generator.hpp                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 17:35:54 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/25 19:13:25 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RANDOM_2D_COORDINATE_GENERATOR_HPP
#define RANDOM_2D_COORDINATE_GENERATOR_HPP

class Random2DCoordinateGenerator {
	private:
		long long _seed;

	public:
		Random2DCoordinateGenerator(long long seed);
		~Random2DCoordinateGenerator() = default;

		long long seed() const;
		long long operator()(const long long& x, const long long& y) const;
};

#endif