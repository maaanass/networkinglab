// udp server program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5000
#define MAXLINE 1000

// Driver code
int main() {
    char send_buffer[MAXLINE];
    char recv_buffer[MAXLINE];
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    // clear servaddr
    bzero(&servaddr, sizeof(servaddr));

    // create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // listen on all interfaces
    servaddr.sin_port = htons(PORT);

    // bind server address to socket descriptor
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }

    while (1) {
        // receive datagram from client
        int len = sizeof(cliaddr);
        int n = recvfrom(sockfd, recv_buffer, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len);
        recv_buffer[n] = '\0';
        printf("Client: %s\n", recv_buffer);

        // exit if "exit" is received
        if (strcmp(recv_buffer, "exit") == 0) {
            printf("Client exited. Exiting...\n");
            break;
        }

        // send response to client
        printf("Enter message: ");
        fgets(send_buffer, MAXLINE, stdin);
        send_buffer[strlen(send_buffer) - 1] = '\0'; // Removing newline character
        
        // send message to client
        sendto(sockfd, (const char *)send_buffer, strlen(send_buffer), 0, (const struct sockaddr *) &cliaddr, len);
        
        // exit if "exit" is entered
        if (strcmp(send_buffer, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }
    }

    // close the socket
    close(sockfd);
    return 0;
}

