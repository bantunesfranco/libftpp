/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   singleton_test.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/01 19:17:20 by bfranco       #+#    #+#                 */
/*   Updated: 2024/11/01 22:54:25 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <string>
#include <iostream>
#include <type_traits>

class Data : public Singleton<Data>
{
	private:
		int _a;
		std::string _b;
	
	public:
		friend class Singleton;

		Data(int a, std::string b) : _a(a), _b(b) {};
		~Data() {};
		
		static void	instance() = delete;
		void		print() {std::cout << "a: " << _a << " b: " << _b << std::endl;}
};

void assert(bool condition, const std::string& message) {
	if (!condition) throw std::runtime_error(message);
}

int main()
{
	Data* data = Singleton<Data>::instance();
	assert(data == nullptr, "Instance should not exist");

	Data::instanciate(1, "hello");

	std::string error = "";
	try {
		Data::instanciate(2, "Goodbye");
	}
	catch (std::exception& e ){
		error.assign(e.what());
	}
	assert(error != "", "Exception was not thrown");

	data = Singleton<Data>::instance();
	Data* other = Singleton<Data>::instance();

	assert(data == other, "Adress should be equal");

	data->print();
	other->print();
}