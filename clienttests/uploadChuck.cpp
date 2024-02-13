#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

std::string createMultipartFormData(const std::string& fileName, const std::string& boundary) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return "";
    }

    std::stringstream requestData;
    requestData << "--" << boundary << "\r\n";
    requestData << "Content-Disposition: form-data; name=\"file\"; filename=\"" << fileName << "\"\r\n";
    requestData << "Content-Type: application/octet-stream\r\n\r\n";
    requestData << file.rdbuf() << "\r\n";
    requestData << "--" << boundary << "--\r\n";

    return requestData.str();
}

int main() {
    // Server information
    const char* serverAddress = "127.0.0.1";
    const int serverPort = 18000;

    // File to be uploaded
    const std::string fileName = "alex.txt";

    // Multipart boundary
    const std::string boundary = "131231232323";

    // Create multipart/form-data request
    std::string multipartData = createMultipartFormData(fileName, boundary);

    // Create chunked request
    std::stringstream request;
    request << "POST /uploads/ HTTP/1.1\r\n";
    request << "Host: " << serverAddress << ":" << serverPort << "\r\n";
    request << "Content-Type: multipart/form-data; boundary=" << boundary << "\r\n";
    request << "Transfer-Encoding: chunked\r\n\r\n";

    // Send chunked header
    request << std::hex << multipartData.size() << "\r\n";
    request << multipartData << "\r\n";
    request << "0\r\n\r\n"; // Final chunk

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket" << std::endl;
        return 1;
    }

    // Resolve server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverAddress, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return 1;
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }

    // Send request
    if (send(sockfd, request.str().c_str(), request.str().size(), 0) < 0) {
        std::cerr << "Error sending request" << std::endl;
        return 1;
    }

    // Receive response
    char buffer[1024];
    int bytesReceived;
    while ((bytesReceived = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        std::cout.write(buffer, bytesReceived);
    }

    // Close socket
    close(sockfd);

    return 0;
}
