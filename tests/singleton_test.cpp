/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   singleton_test.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/01 19:17:20 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/24 20:31:29 by bfranco       ########   odam.nl         */
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
		Data(int a, std::string b) : _a(a), _b(b) {};
		~Data() {};
		
		void	setInt(int n) {_a = n;};
		void	setString(const std::string& str) {_b = str;};
		void	print() {std::cout << "a: " << _a << " b: " << _b << std::endl;}
};

void assert(bool condition, const std::string& message) {
	if (!condition) throw std::runtime_error(message);
}

int main()
{
	assert(Singleton<int>::instance() == nullptr, "Instance should not exist");

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
	Data* newData = Data::instance();

	assert(data == other, "Adress should be equal");
	assert(data == newData, "Adress should be equal");
	assert(newData == other, "Adress should be equal");

	data->print();
	
	other->setInt(42);
	newData->setString("Hello from Codam");

	data->print();

} 