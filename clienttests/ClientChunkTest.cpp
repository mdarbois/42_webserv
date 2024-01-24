#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

const char* server_address = "127.0.0.1";
const int server_port = 18000;

void sendChunk(int socket, const std::string& chunk) {
    // Calculate the size of the chunk in hexadecimal
    std::stringstream chunkSizeStream;
    chunkSizeStream << std::hex << chunk.size();
    std::string chunkSize = chunkSizeStream.str();

    // Construct the chunked data
    std::string chunkedData = chunkSize + "\r\n" + chunk + "\r\n";
	std::cout << chunkedData << std::endl;
	if (chunkedData.find("0\r\n\r\n") != std::string::npos) {
   	 std::cout << "End of chunked data" << std::endl;
	}
    // Send the chunked data to the server
    send(socket, chunkedData.c_str(), chunkedData.size(), 0);
	fflush(stdout);
}

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    // Set up server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_address, &(serverAddr.sin_addr));

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to server");
        close(clientSocket);
        return -1;
    }

    // HTTP POST request data
    std::string postData = "This is the POST data.";

    // Construct the HTTP POST request headers
    std::string requestHeaders = "POST / HTTP/1.1\r\n";
    requestHeaders += "Host: 127.0.0.1:18000\r\n";
    requestHeaders += "Content-Type: application/octet-stream\r\n";
    requestHeaders += "Transfer-Encoding: chunked\r\n\r\n";

    // Send the request headers
    send(clientSocket, requestHeaders.c_str(), requestHeaders.size(), 0);
	fflush(stdout);
	std::cout << requestHeaders << std::endl;
    // Send data in chunks every 2 seconds
    size_t chunkSize = 20;  // Adjust this based on your actual requirements

    for (size_t i = 0; i < postData.size(); i += chunkSize) {
        std::string chunk = postData.substr(i, chunkSize);
        sendChunk(clientSocket, chunk);

        // Sleep for 2 seconds
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    // Send the last chunk with size 0 to indicate the end
    sendChunk(clientSocket, "");

    // Close the socket
    close(clientSocket);

    return 0;
}
