/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ivector3.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/23 13:12:19 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/23 14:38:11 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/math/ivector3.hpp"

/*
	Not sure why this is required since its a templated struct.
	All methods require the use of IVector3<TType> which means
	they need to be place in the .hpp or .tpp file, not the .cpp
*/
