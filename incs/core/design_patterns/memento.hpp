/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memento.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/30 20:00:33 by bfranco       #+#    #+#                 */
/*   Updated: 2024/10/31 19:27:44 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMENTO_HPP
#define MEMENTO_HPP

#include <any>
#include "core/data_structures.hpp"

class Memento
{
	public:
		class Snapshot
		{
			private:
				friend class Memento;
				DataBuffer	_buffer;
			
			public:
				Snapshot() = default;
				~Snapshot() = default;
				Snapshot(const Snapshot& other);
				Snapshot& operator=(const Snapshot& other);

				template<typename T>
				Snapshot& operator>>(T& state);
				template<typename T>
				Snapshot& operator<<(const T& state);
		};

		Memento() = default;
		~Memento() = default;
		
		Snapshot		save();
		void			load(Snapshot& state);

	protected:
		virtual void	_saveToSnapshot(Snapshot& snapshot) = 0;
		virtual void	_loadFromSnapshot(Snapshot& snapshot) = 0;
};

template<typename T>
Memento::Snapshot& Memento::Snapshot::operator<<(const T& state)
{
	_buffer << state;
	return *this;
}

template<typename T>
Memento::Snapshot& Memento::Snapshot::operator>>(T& state)
{
	_buffer >> state;
	return *this;
}

#endif