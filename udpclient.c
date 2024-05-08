// udp client driver program 
#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h> 

#define PORT 5000 
#define MAXLINE 1000 

// Driver code 
int main() { 
    char send_buffer[MAXLINE]; 
    char recv_buffer[MAXLINE];
    int sockfd, n; 
    struct sockaddr_in servaddr; 
    
    // create datagram socket 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    
    // clear servaddr 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET; 
    
    while (1) {
        printf("Enter message: ");
        fgets(send_buffer, MAXLINE, stdin);
        send_buffer[strlen(send_buffer) - 1] = '\0'; // Removing newline character
        
        // send message to server
        sendto(sockfd, send_buffer, strlen(send_buffer), 0, (struct sockaddr*) &servaddr, sizeof(servaddr)); 
        
        // exit if "exit" is entered
        if (strcmp(send_buffer, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }
        
        // receive message from server
        n = recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, NULL, NULL);
        recv_buffer[n] = '\0';
        printf("Server: %s\n", recv_buffer);
        
        // exit if "exit" is received
        if (strcmp(recv_buffer, "exit") == 0) {
            printf("Server exited. Exiting...\n");
            break;
        }
    }
    
    // close the descriptor 
    close(sockfd); 
    return 0;
} 

