// rpc_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8888
#define BUFFER_SIZE 1024

SOCKET client_socket;

// Client stubs for remote procedures
int rpc_connect(char* server_ip) {
    WSADATA wsa;
    struct sockaddr_in server_addr;
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed\n");
        return 0;
    }
    
    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 0;
    }
    
    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    
    // Connect
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed. Error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 0;
    }
    
    // Receive welcome message
    char buffer[BUFFER_SIZE];
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s\n\n", buffer);
    
    return 1;
}

int rpc_add(int a, int b) {
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int result;
    
    sprintf(request, "ADD %d %d", a, b);
    send(client_socket, request, strlen(request), 0);
    
    recv(client_socket, response, BUFFER_SIZE, 0);
    sscanf(response, "RESULT %d", &result);
    
    return result;
}

int rpc_subtract(int a, int b) {
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int result;
    
    sprintf(request, "SUB %d %d", a, b);
    send(client_socket, request, strlen(request), 0);
    
    recv(client_socket, response, BUFFER_SIZE, 0);
    sscanf(response, "RESULT %d", &result);
    
    return result;
}

int rpc_multiply(int a, int b) {
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int result;
    
    sprintf(request, "MUL %d %d", a, b);
    send(client_socket, request, strlen(request), 0);
    
    recv(client_socket, response, BUFFER_SIZE, 0);
    sscanf(response, "RESULT %d", &result);
    
    return result;
}

float rpc_divide(int a, int b) {
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    float result;
    
    sprintf(request, "DIV %d %d", a, b);
    send(client_socket, request, strlen(request), 0);
    
    recv(client_socket, response, BUFFER_SIZE, 0);
    sscanf(response, "RESULT %f", &result);
    
    return result;
}

char* rpc_get_message(char* name) {
    static char result[BUFFER_SIZE];
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    
    sprintf(request, "MSG %s", name);
    send(client_socket, request, strlen(request), 0);
    
    recv(client_socket, response, BUFFER_SIZE, 0);
    sscanf(response, "RESULT %[^\n]", result);
    
    return result;
}

void rpc_disconnect() {
    send(client_socket, "EXIT", 4, 0);
    closesocket(client_socket);
    WSACleanup();
}

int main(int argc, char* argv[]) {
    char server_ip[16] = "127.0.0.1";
    
    if (argc > 1) {
        strcpy(server_ip, argv[1]);
    }
    
    printf("========================================\n");
    printf("RPC Client - Connecting to %s:%d\n", server_ip, PORT);
    printf("========================================\n\n");
    
    // Connect to RPC server
    if (!rpc_connect(server_ip)) {
        printf("Failed to connect to server\n");
        return 1;
    }
    
    printf("Connected successfully!\n\n");
    printf("Calling remote procedures:\n");
    printf("--------------------------\n");
    
    // Call remote procedures
    int sum = rpc_add(10, 20);
    printf("RPC Call: 10 + 20 = %d\n", sum);
    
    int diff = rpc_subtract(50, 30);
    printf("RPC Call: 50 - 30 = %d\n", diff);
    
    int prod = rpc_multiply(7, 8);
    printf("RPC Call: 7 * 8 = %d\n", prod);
    
    float quot = rpc_divide(100, 3);
    printf("RPC Call: 100 / 3 = %.2f\n", quot);
    
    char* msg = rpc_get_message("John");
    printf("RPC Call: get_message(\"John\") = %s\n", msg);
    
    printf("\nAll RPC calls successful!\n");
    
    // Disconnect
    rpc_disconnect();
    printf("\nDisconnected from server.\n");
    
    return 0;
}