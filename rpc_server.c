// rpc_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8888
#define BUFFER_SIZE 1024

// Remote procedures that can be called by client
int add(int a, int b) {
    printf("[RPC] Executing add(%d, %d)\n", a, b);
    return a + b;
}

int subtract(int a, int b) {
    printf("[RPC] Executing subtract(%d, %d)\n", a, b);
    return a - b;
}

int multiply(int a, int b) {
    printf("[RPC] Executing multiply(%d, %d)\n", a, b);
    return a * b;
}

float divide(int a, int b) {
    printf("[RPC] Executing divide(%d, %d)\n", a, b);
    if (b == 0) return 0;
    return (float)a / b;
}

char* get_message(char* name) {
    printf("[RPC] Executing get_message(%s)\n", name);
    static char message[BUFFER_SIZE];
    sprintf(message, "Hello %s! Welcome to RPC Server.", name);
    return message;
}

// Process RPC request
void process_rpc_request(char* request, char* response) {
    char method[32];
    int a, b;
    float result_f;
    char name[64];
    
    // Parse the request
    if (sscanf(request, "ADD %d %d", &a, &b) == 2) {
        int result = add(a, b);
        sprintf(response, "RESULT %d", result);
    }
    else if (sscanf(request, "SUB %d %d", &a, &b) == 2) {
        int result = subtract(a, b);
        sprintf(response, "RESULT %d", result);
    }
    else if (sscanf(request, "MUL %d %d", &a, &b) == 2) {
        int result = multiply(a, b);
        sprintf(response, "RESULT %d", result);
    }
    else if (sscanf(request, "DIV %d %d", &a, &b) == 2) {
        float result = divide(a, b);
        sprintf(response, "RESULT %.2f", result);
    }
    else if (sscanf(request, "MSG %[^\n]", name) == 1) {
        char* result = get_message(name);
        sprintf(response, "RESULT %s", result);
    }
    else {
        sprintf(response, "ERROR Unknown method");
    }
}

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    
    printf("[Server] Client connected\n");
    
    // Send welcome message
    char* welcome = "WELCOME to RPC Server. Available methods: ADD, SUB, MUL, DIV, MSG";
    send(client_socket, welcome, strlen(welcome), 0);
    
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        
        // Receive RPC request
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("[Server] Client disconnected\n");
            break;
        }
        
        printf("[Server] Received request: %s\n", buffer);
        
        // Check for exit
        if (strcmp(buffer, "EXIT") == 0) {
            printf("[Server] Client requested exit\n");
            break;
        }
        
        // Process RPC request
        process_rpc_request(buffer, response);
        
        // Send response
        send(client_socket, response, strlen(response), 0);
        printf("[Server] Sent response: %s\n", response);
    }
    
    closesocket(client_socket);
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    
    // Initialize Winsock
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed. Error: %d\n", WSAGetLastError());
        return 1;
    }
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed. Error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    
    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    // Listen
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("Listen failed. Error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    printf("\n========================================\n");
    printf("RPC Server running on port %d\n", PORT);
    printf("Waiting for client connections...\n");
    printf("========================================\n\n");
    
    while (1) {
        // Accept client
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed. Error: %d\n", WSAGetLastError());
            continue;
        }
        
        printf("[Server] Client connected from %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        // Handle client in a loop (single client at a time for simplicity)
        handle_client(client_socket);
        
        printf("[Server] Ready for next client...\n\n");
    }
    
    closesocket(server_socket);
    WSACleanup();
    return 0;
}