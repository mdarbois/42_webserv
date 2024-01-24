#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const char* targetHost = "127.0.0.1";
const int targetPort = 18001;
const char* targetPath = "/post-endpoint";
const char* postData = "key1=value1&key2=value2";

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        return 1;
    }

    // Set up server address structure
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(targetPort);
    inet_pton(AF_INET, targetHost, &serverAddress.sin_addr);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error connecting to server");
        close(clientSocket);
        return 1;
    }

    // Prepare the HTTP POST request
    std::string httpRequest =
        "POST " + std::string(targetPath) + " HTTP/1.1\r\n"
        "Host: " + std::string(targetHost) + "\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: " + std::to_string(strlen(postData)) + "\r\n"
        "\r\n" + std::string(postData);

    // Send the HTTP POST request
    if (send(clientSocket, httpRequest.c_str(), httpRequest.length(), 0) == -1) {
        perror("Error sending data");
        close(clientSocket);
        return 1;
    }

    // Receive and print the response
    char buffer[4096];
    std::string response;
    ssize_t bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, bytesRead);
    }

    // Print the HTTP response
    std::cout << "HTTP Response:\n" << response << std::endl;

    // Close the socket
    close(clientSocket);

    return 0;
}
