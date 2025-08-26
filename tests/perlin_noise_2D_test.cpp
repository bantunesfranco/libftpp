/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   perlin_noise_2D_test.cpp                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 20:34:25 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/26 21:06:25 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include "libftpp.hpp"
#include <sys/stat.h>
#include <sys/types.h>


void assert(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

void generate_img(int n, std::string data) {
	std::string outfile = "resources/csv/noise" + std::to_string(n) + ".csv";

	mkdir("resources/csv", 0777);

	std::ofstream file(outfile);
	file << data;
	file.close();

	std::string cmd = "python3 resources/viz.py " + outfile;
	std::system(cmd.c_str());
}

void create_noise(int n)
{
	float map[100][100] = {};
	
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			map[i][j] = -42;
		}
	}

	PerlinNoise2D noise(42 + n);
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			float ni = i / 100.0f;
			float nj = j / 100.0f;

			if (j % 2 == 0)
				map[i][j] = noise(ni, nj);
			else
				map[i][j] = noise.sample(ni, nj);

			assert(map[i][j] != -42, "Noise should not be -42");
			assert(map[i][j] >= -1 && map[i][j] <= 1, "Noise should be between -1 and 1");
		}
	}

	std::string out = "";
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			out += std::to_string(map[i][j]);
			if (j < 99) out += ",";
		}
		out += "\n";
	}

	generate_img(n, out);
}

int main() {
	for (int i = 0; i < 25; ++i) {
		create_noise(i);
	}
	return 0;
}