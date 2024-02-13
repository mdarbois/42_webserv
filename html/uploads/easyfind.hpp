/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easyfind.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:45:57 by aehrlich          #+#    #+#             */
/*   Updated: 2023/12/19 16:41:11 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EASYFIND_H
# define EASYFIND_H

#include <algorithm>

class ElementNotInContainer: public std::exception
{
	public:
		virtual const char* what() const throw() { return "Element not in container."; }
};

/*
	Returns the index of the first occurence of target. If not, an exception is thrown.
*/
template <typename T>
int	easyfind(T& container, int target)
{
	typename T::iterator	it;

	it = std::find(container.begin(), container.end(), target);
	if (it == container.end())
		throw ElementNotInContainer();
	return (std::distance(container.begin(), it));
}

#endif
