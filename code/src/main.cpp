/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 09:18:24 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 09:24:33 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int	main(int argc, char **argv)
{
	std::string	configFile;

	if (argc == 1)
		configFile = "Path to default config file";
	else if (argc == 2)
		configFile = argv[1];
	else
	{
		std::cerr << "error: wrong number of arguments." << std::endl;
		std::cerr << "Usage: ./webserv <> OR ./webserv <config.txt>" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
