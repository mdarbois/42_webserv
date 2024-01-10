/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 08:59:33 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 09:02:04 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

const int PORT = 18000;
const int MAX_CONNECTIONS = 10;

void handle_request(int client_socket) {
	const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
	send(client_socket, response, strlen(response), 0);
	close(client_socket);
}

int main() {
	int server_socket, client_socket;
	sockaddr_in server_addr{}, client_addr{};
	socklen_t client_addr_len = sizeof(client_addr);

	// Create socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		std::cerr << "Error creating socket\n";
		exit(EXIT_FAILURE);
	}

	// Set up server address structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	// Bind the socket
	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		std::cerr << "Error binding socket\n";
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections
	if (listen(server_socket, MAX_CONNECTIONS) == -1) {
		std::cerr << "Error listening for connections\n";
		close(server_socket);
		exit(EXIT_FAILURE);
	}

	std::cout << "Server listening on port " << PORT << "...\n";

	while (true) {
		// Accept connection
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
		if (client_socket == -1) {
			std::cerr << "Error accepting connection\n";
			continue;
		}

		// Handle request in a separate function
		handle_request(client_socket);
	}

	// Close the server socket
	close(server_socket);

	return 0;
}
