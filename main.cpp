/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 12:29:15 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 12:29:20 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "Parsing.hpp"
#include "./includes/Server.hpp"

int main(int argc, char **argv)
{
	if (argc < 1 || argc > 2)
	{
		std::cout << "Invalid number of arguments" << std::endl;
		return (1);
	}
	/* else
	{
		try
		{
			std::string path;
			if (argc == 1)
				path = "configuration_file.txt";
			else
				path = argv[1];
			Parsing parsing(path);

		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return (1);
		}
	} */




	return (0);
}
