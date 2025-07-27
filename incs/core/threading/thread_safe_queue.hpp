/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread_safe_queue.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 14:06:22 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/25 19:00:57 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <deque>
#include <mutex>

template <typename TType>
class ThreadSafeQueue : public std::deque<TType>
{
	private:
		std::mutex _mutex;
	public:
		ThreadSafeQueue() = default;
		~ThreadSafeQueue() = default;
		ThreadSafeQueue(const ThreadSafeQueue& other) { *this = other; }
		ThreadSafeQueue& operator=(const ThreadSafeQueue& other) {
			if (this != &other) {
				std::lock_guard<std::mutex> thisLock(_mutex);
				std::lock_guard<std::mutex> otherLock(other._mutex);
				std::deque<TType>::operator=(other);
			}
			return *this;
		}

		void push_back(const TType& newElement);
		void push_front(const TType& newElement);
		TType pop_back();
		TType pop_front();
};

template <typename TType>
void ThreadSafeQueue<TType>::push_back(const TType& newElement)
{
	std::lock_guard<std::mutex> lock(_mutex);
	std::deque<TType>::push_back(newElement);
}

template <typename TType>
void ThreadSafeQueue<TType>::push_front(const TType& newElement)
{
	std::lock_guard<std::mutex> lock(_mutex);
	std::deque<TType>::push_front(newElement);
}

template <typename TType>
TType ThreadSafeQueue<TType>::pop_back()
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (std::deque<TType>::empty())
		throw std::out_of_range("Queue is empty");
	TType value = std::deque<TType>::back();
	std::deque<TType>::pop_back();
	return value;
}

template <typename TType>
TType ThreadSafeQueue<TType>::pop_front() 
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (std::deque<TType>::empty())
		throw std::out_of_range("Queue is empty");
	TType value = std::deque<TType>::front();
	std::deque<TType>::pop_front();
	return value;
}

#endif