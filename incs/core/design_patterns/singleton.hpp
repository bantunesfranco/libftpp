/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   singleton.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/01 18:19:04 by bfranco       #+#    #+#                 */
/*   Updated: 2024/11/01 22:45:48 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>
#include <stdexcept>

template<typename TType>
class Singleton
{
	private:
		static std::unique_ptr<TType>	_instance;

	protected:
		Singleton() = default;
		virtual ~Singleton() = default;
		Singleton(const Singleton& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;


	public:
		template<typename ... TArgs>
		static void		instanciate(TArgs&& ... p_args);
		static TType*	instance();
};

template<typename TType>
std::unique_ptr<TType> Singleton<TType>::_instance = nullptr;

template<typename TType>
TType*	Singleton<TType>::instance()
{
	return _instance.get();
}

template<typename TType>
template<typename ... TArgs>
void	Singleton<TType>::instanciate(TArgs&& ... p_args)
{
	if (_instance.get() != nullptr)
		throw std::runtime_error("Instance already exists");
	Singleton::_instance = std::make_unique<TType>(std::forward<TArgs>(p_args)...);
}

#endif