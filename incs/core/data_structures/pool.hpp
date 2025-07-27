/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pool.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/25 17:39:42 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/27 22:59:20 by bfranco       ########   odam.nl         */
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
				Pool*					_pool;
				size_t					_index;
				std::shared_ptr<TType>	_ptr;

			public:
				Object(Pool* pool, TType* ptr, size_t index) : _pool(pool), _index(index) {
					_ptr = std::shared_ptr<TType>(ptr, [pool, index](TType* p) {
    				    p->~TType();  pool->release(index);
    				});
				}
				~Object() = default;
				
				Object(const Object &other) { *this = other; }
				Object	&operator=(const Object &other) {
					if (this != &other) {
						auto tmp = other._pool->acquire(other._ptr);
						_pool = tmp._pool;
						_index = tmp._index;
						_ptr = std::move(tmp._ptr);
					}
					return *this;
				}

				TType* operator->() { return _ptr.get(); };
				TType& operator*() { return *_ptr; };
		};
		
		Pool(size_t poolSize = 5);
		~Pool();

		template<typename ... TArgs>
		Object	acquire(TArgs&&... p_args);
		void	resize(const size_t numberOfObjectStored);
		void	release(size_t index);
		size_t	size() const;
		void	add(Object& obj);
		
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
void Pool<TType>::release(size_t index) { _available.push(index); }

template<typename TType>
template<typename ... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... p_args)
{
	if (_available.empty())
		resize(_poolSize + 1);

	size_t	index = _available.top();
	_available.pop();

	void* mem = &_pool[index];
	TType* objPtr = new (mem) TType(std::forward<decltype(TArgs(p_args))>(p_args)...);
	return Object(this, objPtr, index);
}

template<typename TType>
size_t Pool<TType>::size() const { return _poolSize; }


#endif