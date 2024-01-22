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


const char* boundary = "---------------------------BOUNDARY_STRING";

void send_multipart_request(int client_socket) {
    std::string request_data = boundary + std::string("\r\n") +
                               "Content-Disposition: form-data; name=\"field1\"\r\n" +
                               "\r\n" +
                               "value1\r\n" +
                               boundary + "\r\n" +
                               "Content-Disposition: form-data; name=\"field2\"\r\n" +
                               "\r\n" +
                               "value2\r\n" +
                               boundary + "\r\n" +
                               "Content-Disposition: form-data; name=\"file\"; filename=\"example.txt\"\r\n" +
                               "Content-Type: text/plain\r\n" +
                               "\r\n" +
                               "Contents of the file...\r\n" +
                               boundary + "--\r\n";

    std::string request =
        "POST /example/form-endpoint HTTP/1.1\r\n"
        "Host: 127.0.0.1:18000\r\n"
        "Content-Type: multipart/form-data; boundary=---------------------------BOUNDARY_STRING\r\n"
        "Content-Length: " + std::to_string(request_data.length()) + "\r\n"
        "\r\n" +
        request_data;

    // Send the request to the server
    send(client_socket, request.c_str(), request.length(), 0);
}

int main() {
    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Specify the server address and port
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(18000);
    inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr));

    // Connect to the server
    connect(client_socket, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address));

    // Send the multipart request
    send_multipart_request(client_socket);

    // Receive and print the response (you may want to implement proper handling here)
    char buffer[4096];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes_received] = '\0';
    std::cout << "Response:\n" << buffer << std::endl;

    // Close the socket
    close(client_socket);

    return 0;
}
