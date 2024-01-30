#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int PORT = 18000;
const char* SERVER_IP = "127.0.0.1";
const int BUFFER_SIZE = 1024;

// Function to read the content of a file into a string
std::string readFile(const std::string& filename) {
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (file) {
        std::ostringstream content;
        content << file.rdbuf();
        return content.str();
    } else {
        // Handle error: Unable to open file
        std::cerr << "Error reading file: " << filename << std::endl;
        return "";
    }
}

// Function to create a multipart/form-data POST request
std::string createMultipartFormDataRequest(const std::string& filename, const std::string& fileContent) {
    std::string boundary = "--boundary";
    std::string requestBody;

    // Build request body
    requestBody += boundary + "\r\n";
    requestBody += "Content-Disposition: form-data; name=file; filename=" + filename + "\r\n";
    requestBody += "Content-Type: text/plain\r\n\r\n";
    requestBody += fileContent + "\r\n";
    requestBody += boundary + "--\r\n";

    // Build request headers
    std::ostringstream request;
    request << "POST / HTTP/1.1\r\n";
    request << "Host: " << SERVER_IP << ":" << PORT << "\r\n";
    request << "Content-Type: multipart/form-data; boundary=" << boundary << "\r\n";
    request << "Content-Length: " << requestBody.length() << "\r\n\r\n";
    request << requestBody;

    return request.str();
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddress.sin_port = htons(PORT);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Read the content of the file
    std::string filename = "alex.txt";
    std::string fileContent = readFile(filename);

    // Create multipart/form-data request
    std::string request = createMultipartFormDataRequest(filename, fileContent);

    // Send the request to the server
    send(clientSocket, request.c_str(), request.length(), 0);

    // Receive and print the server's response (you may modify this part as needed)
    char buffer[BUFFER_SIZE];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        std::cout << "Server Response:\n" << std::string(buffer, bytesRead) << std::endl;
    } else {
        std::cerr << "Error receiving server response" << std::endl;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
