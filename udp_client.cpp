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
    int sockfd;
    struct sockaddr_in servaddr;
    std::string message = "Hello from UDP Client!";
    char buffer[1024];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(HOST);

    unsigned int n, len = sizeof(servaddr);

    sendto(sockfd, (const char *)message.c_str(), message.length(), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    std::cout << "Message sent to UDP server.\n";

    n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    std::cout << "Response from server: " << buffer << "\n";

    close(sockfd);
    return 0;
}
