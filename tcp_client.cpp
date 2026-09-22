#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 65432
#define HOST "192.168.2.1"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    std::string message = "Hello from TCP Client!";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "\nSocket creation error \n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, HOST, &serv_addr.sin_addr) <= 0) {
        std::cout << "\nInvalid address / Address not supported \n";
        return -1;
    }

    std::cout << "Connecting to TCP server at " << HOST << ":" << PORT << "...\n";
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "\nConnection Failed \n";
        return -1;
    }

    send(sock, message.c_str(), message.length(), 0);
    std::cout << "Message sent to server\n";
    
    read(sock, buffer, 1024);
    std::cout << "Response from server: " << buffer << "\n";

    close(sock);
    return 0;
}
