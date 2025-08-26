/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   random_2D_coordinates_generator_test.cpp           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 19:08:05 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/25 19:22:29 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <random>
#include <chrono>
#include "libftpp.hpp"

void assert(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

int main() {
	Random2DCoordinateGenerator generator(42), generator2(42);

	std::vector<long long> coords, coords2;

	for (int i = 0; i < 10; ++i) {
		coords.push_back(generator(i, i));
		coords2.push_back(generator2(i, i));
	}
	
	std::cout << "Seed: " << generator.seed() << std::endl;
	std::cout << "Seed2: " << generator2.seed() << std::endl;

	for (int i = 0; i < 10; ++i) {
		std::cout << coords[i] << " " << coords2[i] << std::endl;
		assert(coords[i] == coords2[i], "Coordinates should be the same");
	}

	std::cout << std::endl;

	std::srand(std::time(nullptr));
	Random2DCoordinateGenerator generator3(std::rand());

	std::cout << "Seed3: " << generator3.seed() << std::endl;
	
	for (int i = 0; i < 10; ++i) {
		std::cout << generator3(i/2, i*2) << std::endl;
	}


	return 0;
}