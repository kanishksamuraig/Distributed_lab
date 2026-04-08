// windows_tcp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
//gcc client.c -o client.exe -lws2_32

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    WSADATA wsa;
    SOCKET client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char server_ip[16] = "127.0.0.1";
    
    if (argc > 1) {
        strcpy(server_ip, argv[1]);
    }
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed\n");
        return 1;
    }
    
    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }
    
    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    
    // Connect
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed\n");
        return 1;
    }
    
    printf("Connected to server at %s:%d\n", server_ip, PORT);
    
    // Communication loop
    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        
        send(client_fd, buffer, strlen(buffer), 0);
        
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_read <= 0) {
            printf("Server disconnected\n");
            break;
        }
        
        printf("Server: %s\n", buffer);
    }
    
    closesocket(client_fd);
    WSACleanup();
    
    return 0;
}