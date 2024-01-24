#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

const char* server_address = "127.0.0.1";
const int server_port = 18000;
const char* cgi_script = "/test.py";

const char* post_data = "param1=value1&param2=value2&param3=value3";

int main() {
    // Create a TCP socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        return 1;
    }

    // Resolve the server address
    struct hostent* server_info = gethostbyname(server_address);
    if (server_info == nullptr) {
        perror("Error resolving server address");
        close(client_socket);
        return 1;
    }

    // Set up server address structure
    struct sockaddr_in server_address_struct;
    memset(&server_address_struct, 0, sizeof(server_address_struct));
    server_address_struct.sin_family = AF_INET;
    server_address_struct.sin_port = htons(server_port);
    memcpy(&server_address_struct.sin_addr.s_addr, server_info->h_addr, server_info->h_length);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address_struct, sizeof(server_address_struct)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        return 1;
    }

    // Build the HTTP POST request
    std::string http_request = "POST " + std::string(cgi_script) + " HTTP/1.1\r\n";
    http_request += "Host: " + std::string(server_address) + "\r\n";
    http_request += "Content-Type: application/x-www-form-urlencoded\r\n";
    http_request += "Content-Length: " + std::to_string(strlen(post_data)) + "\r\n";
    http_request += "\r\n";
    http_request += post_data;

    // Send the HTTP request
    if (send(client_socket, http_request.c_str(), http_request.length(), 0) == -1) {
        perror("Error sending request");
        close(client_socket);
        return 1;
    }

    // Receive and print the response
    char buffer[1024];
    ssize_t bytes_received;
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytes_received, stdout);
    }

    // Close the socket
    close(client_socket);

    return 0;
}
