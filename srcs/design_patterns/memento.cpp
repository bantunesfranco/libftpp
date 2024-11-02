/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memento.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/30 19:59:39 by bfranco       #+#    #+#                 */
/*   Updated: 2024/10/31 19:28:00 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/design_patterns.hpp"

using Snapshot = Memento::Snapshot;

Snapshot	Memento::save()
{
	Snapshot snapshot;
	_saveToSnapshot(snapshot);
	return snapshot;
}

void	Memento::load(Snapshot& state)
{
	_loadFromSnapshot(state);
}

Snapshot::Snapshot(const Snapshot& other)
{
	if (this != &other)
		*this = other;
}

Snapshot& Snapshot::operator=(const Snapshot& other)
{
	if (this != &other)
		_buffer = other._buffer;
	return *this;
}