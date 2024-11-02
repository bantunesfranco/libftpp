/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memento_test.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/30 20:43:26 by bfranco       #+#    #+#                 */
/*   Updated: 2024/10/31 21:08:26 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <bits/stl_pair.h>
#include <string>
#include <iostream>

class Data : public Memento {
	public:
		int a;
		std::string b;
		
		using Snapshot = typename Memento::Snapshot;

		Data(int a, std::string b) : a(a), b(b) {}
		~Data() = default;
	
	private:
		void _saveToSnapshot(Snapshot& snapshot) override {
			snapshot << a << b;
		}

		void _loadFromSnapshot(Snapshot& snapshot) override {
			snapshot >> b >> a;
		}
		
		friend class Memento;
};

void assert(bool condition, const std::string& message) {
	if (!condition) throw std::runtime_error(message);
}

int main() {
	Data data(1, "example");
	Data::Snapshot snapshot;
	Data::Snapshot snapshot2;

	snapshot = data.save();

	data.a = 0;
	data.b = "changed";
	assert(data.a == 0, "Data a should be 0");
	assert(data.b == "changed", "Data b should be 'changed'");

	snapshot2 = data.save();

	data.load(snapshot);
	assert(data.a == 1, "Data a should be 1");
	assert(data.b == "example", "Data b should be 'example'");

	data.load(snapshot2);
	assert(data.a == 0, "Data a should be 0");
	assert(data.b == "changed", "Data b should be 'changed'");

	return 0;
}