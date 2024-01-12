#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>

const int PORT = 18000;
const int MAX_CLIENTS = 10;

const char* RESPONSE = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";

int main() {
	int serverSocket, clientSocket;
	sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	// Create a socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		perror("Error creating socket");
		return -1;
	}

	// Set up the server address struct
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	// Bind the socket
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		perror("Error binding socket");
		close(serverSocket);
		return -1;
	}

	// Listen for incoming connections
	if (listen(serverSocket, MAX_CLIENTS) == -1) {
		perror("Error listening for connections");
		close(serverSocket);
		return -1;
	}

	std::vector<pollfd> fds(MAX_CLIENTS + 1); // Additional slot for the server socket
	fds[0].fd = serverSocket;
	fds[0].events = POLLIN;

	std::cout << "Server listening on port " << PORT << std::endl;

	while (true) {
		int ret = poll(&fds[0], fds.size(), -1);
		if (ret == -1) {
			perror("Error in poll");
			break;
		}

		// Check for incoming connection on the server socket
		if (fds[0].revents & POLLIN) {
			clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
			if (clientSocket == -1) {
				perror("Error accepting connection");
				continue;
			}

			std::cout << "New connection from " << ":" << ntohs(clientAddr.sin_port) << std::endl;

			// Add the new client socket to the list of monitored file descriptors
			for (size_t i = 1; i < fds.size(); ++i) {
				if (fds[i].fd == -1) {
					fds[i].fd = clientSocket;
					fds[i].events = POLLIN;
					break;
				}
			}
		}

		// Check for data on client sockets
		for (size_t i = 1; i < fds.size(); ++i) {
			if (fds[i].revents & POLLIN) {
				// Receive data from the client
				char buffer[1024];
				ssize_t bytesRead = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
				if (bytesRead <= 0) {
					std::cout << "Client disconnected\n";
					close(fds[i].fd);
					fds[i].fd = -1;
				} else {
					buffer[bytesRead] = '\0';
					std::cout << "Received data from client: " << buffer << std::endl;

					// Check for a valid HTTP GET request
					if (strstr(buffer, "GET") != nullptr) {
						// Send the HTTP response
						send(fds[i].fd, RESPONSE, strlen(RESPONSE), 0);
					}

					// Close the connection after sending the response
					close(fds[i].fd);
					fds[i].fd = -1;
				}
			}
		}
	}

	// Close all client sockets
	for (size_t i = 1; i < fds.size(); ++i) {
		if (fds[i].fd != -1) {
			close(fds[i].fd);
		}
	}

	// Close the server socket
	close(serverSocket);

	return 0;
}
