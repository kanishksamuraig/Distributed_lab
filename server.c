// windows_tcp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed\n");
        return 1;
    }
    
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }
    
    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        return 1;
    }
    
    // Listen
    listen(server_fd, 5);
    
    printf("Windows TCP Server listening on port %d...\n", PORT);
    
    // Accept connection
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == INVALID_SOCKET) {
        printf("Accept failed\n");
        return 1;
    }
    
    printf("Client connected from %s:%d\n", 
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    // Communication loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        
        int bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_read <= 0) {
            printf("Client disconnected\n");
            break;
        }
        
        printf("Client: %s\n", buffer);
        
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        
        send(client_fd, buffer, strlen(buffer), 0);
    }
    
    closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();
    
    return 0;
}