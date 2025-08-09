/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   worker_pool_test.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 16:45:28 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/28 17:12:24 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"

int main()
{
	WorkerPool pool(5);

	pool.addJob("square", [&](){static int i = 0; ThreadSafeIOStream::threadSafeCout << i*i << std::endl; ++i;});

	pool.removeTask("square");
}