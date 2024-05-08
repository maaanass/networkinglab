#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>

#define PORT 8094
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 2

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_socket, 5);
    printf("Server listening on port %d...\n", PORT);

    // Accept a connection from a client
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
    if (client_socket == -1)
    {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Receive frame from the client
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0)
        {
            // printf("Received frame: %s\n", buffer);

            // Simulate data loss
            if ((float)rand() / RAND_MAX < 0.3)
            {
                printf("Simulated data loss. Acknowledgment not sent.\n");
                continue;
            }

            // Extract sequence number from frame
            int seq_num;
            sscanf(buffer, "SEQ%d", &seq_num);

            // Send acknowledgment to the client for the received frame
            printf("Sending acknowledgment for frame: SEQ%d\n", seq_num);
            sprintf(buffer, "ACK%d", seq_num);
            send(client_socket, buffer, strlen(buffer), 0);
        }
        else
        {
            printf("Frame not received from client. Connection may be closed.\n");
            break;
        }
    }

    close(server_socket);
    close(client_socket);
    return 0;
}