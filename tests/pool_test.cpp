#include "libftpp.hpp"
#include <iostream>

class Data
{
	public:
		int a;
		int b;

		Data() : a(0), b(0) {}
		Data(int a, int b) : a(a), b(b) {}
		~Data() {}

		void print()
		{
			std::cout << "Data -> a: " << a << ", b: " << b << std::endl;
		}
};

int main()
{
	Pool<Data> pool;

	pool.resize(3);

	auto obj = pool.acquire(2, 3);
	obj->print();

	{
		auto obj2 = pool.acquire(4, 5);
		obj2->print();
	}

	auto obj3 = pool.acquire(6, 7);
	obj3->print();

	return 0;
}