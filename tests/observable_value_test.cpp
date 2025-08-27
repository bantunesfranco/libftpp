/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   observable_value_test.cpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/26 22:29:12 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/26 22:49:06 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"

void assert(bool condition, const std::string& message) {
	if (!condition) throw std::runtime_error(message);
}

int main()
{
	ObservableValue<int> value(42);

	for (int i = 0; i < 5; ++i) { 
		value = value() + 1;
	}
	assert(value.get() == 42 + 5, "value does not match the expected value");
	assert(value() == 42 + 5, "value does not match the expected value");

	ObservableValue<std::string> value2("42");

	value2.set("420");
	assert(value2.get() == "420", "value2 should be 420");
	value2 = value2() + "1";
	assert(value2.get() == "4201", "value2 should be 420");
} 