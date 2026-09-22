#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 65433
#define HOST "192.168.2.1"

int main() {
    int server_fd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    std::string response = "Ack: Message received by UDP Server";

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(HOST);
    servaddr.sin_port = htons(PORT);

    if (bind(server_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "UDP Server listening on " << HOST << ":" << PORT << "...\n";

    unsigned int len = sizeof(cliaddr);
    int n = recvfrom(server_fd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    
    std::cout << "Received message from client: " << buffer << "\n";
    
    sendto(server_fd, (const char *)response.c_str(), response.length(), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    std::cout << "UDP acknowledgement sent.\n";

    close(server_fd);
    return 0;
}
