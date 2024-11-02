/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pool.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/25 17:39:42 by bfranco       #+#    #+#                 */
/*   Updated: 2024/10/30 23:36:08 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOL_HPP
#define POOL_HPP

#include <stddef.h>
#include <vector>
#include <stack>
#include <memory>
#include <iostream>

template<typename TType>
class Pool
{
	private:
		size_t				_poolSize;
		std::vector<TType>	_pool;
		std::stack<size_t>	_available;

	public:
		class Object
		{
			private:
				Pool*		_pool;
				TType*		_ptr;
				size_t		_index;

			public:
				Object(Pool* pool, TType* ptr, size_t index) : _pool(pool), _ptr(ptr), _index(index) {}
				~Object() { if (_ptr) _pool->release(_index); }

				Object(const Object &other) = delete;
				Object	&operator=(const Object &other) = delete;
				Object(Object &other) = delete;
				Object	&operator=(Object &other) = delete;

				TType*	operator->() { return _ptr; }
		};
		
		Pool(size_t poolSize = 5);
		~Pool();

		template<typename ... TArgs>
		Object	acquire(TArgs&&... p_args);
		void	resize(const size_t numberOfObjectStored);
		void	release(size_t index);
};

template<typename TType>
Pool<TType>::Pool(size_t poolSize) : _poolSize(0) { resize(poolSize); }

template<typename TType>
Pool<TType>::~Pool(){}

template<typename TType>
void Pool<TType>::resize(const size_t numberOfObjectStored)
{
	if (numberOfObjectStored <= _poolSize)
		return;

	_pool.resize(numberOfObjectStored);
	for (; _poolSize < numberOfObjectStored; ++_poolSize)
		_available.push(_poolSize);
}

template<typename TType>
void Pool<TType>::release(size_t index)
{
	try
	{
		_pool.at(index).~TType();
		_available.push(index);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

template<typename TType>
template<typename ... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... p_args)
{
	if (_available.empty())
		resize(_poolSize + 1);

	size_t	index = _available.top();
	_available.pop();

	new (&(_pool.at(index))) TType(std::forward<decltype(TArgs(p_args))>(p_args)...);
	return Object(this, &(_pool.at(index)), index);
}

#endif