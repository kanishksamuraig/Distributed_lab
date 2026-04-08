#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    int parent_to_child[2];  // Pipe 1: Parent writes, Child reads
    int child_to_parent[2];  // Pipe 2: Child writes, Parent reads
    pid_t pid;
    char buffer[BUFFER_SIZE];
    
    // Create both pipes
    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        perror("pipe creation failed");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) {  // Parent process
        // Close unused pipe ends
        close(parent_to_child[0]);   // Close read end of parent->child pipe
        close(child_to_parent[1]);   // Close write end of child->parent pipe
        
        // Send message to child
        char *parent_msg = "Hello Child, this is Parent!";
        printf("Parent: Sending - '%s'\n", parent_msg);
        write(parent_to_child[1], parent_msg, strlen(parent_msg) + 1);
        
        // Read response from child
        read(child_to_parent[0], buffer, BUFFER_SIZE);
        printf("Parent: Received from child - '%s'\n", buffer);
        
        // Send another message
        char *parent_msg2 = "How are you doing?";
        printf("Parent: Sending - '%s'\n", parent_msg2);
        write(parent_to_child[1], parent_msg2, strlen(parent_msg2) + 1);
        
        // Read second response
        read(child_to_parent[0], buffer, BUFFER_SIZE);
        printf("Parent: Received from child - '%s'\n", buffer);
        
        // Close remaining pipe ends
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        
        wait(NULL);
        printf("Parent: Communication ended\n");
        
    } else {  // Child process
        // Close unused pipe ends
        close(parent_to_child[1]);   // Close write end of parent->child pipe
        close(child_to_parent[0]);   // Close read end of child->parent pipe
        
        // Read message from parent
        read(parent_to_child[0], buffer, BUFFER_SIZE);
        printf("Child: Received from parent - '%s'\n", buffer);
        
        // Send response to parent
        char *child_msg = "Hello Parent, Child here!";
        printf("Child: Sending - '%s'\n", child_msg);
        write(child_to_parent[1], child_msg, strlen(child_msg) + 1);
        
        // Read second message
        read(parent_to_child[0], buffer, BUFFER_SIZE);
        printf("Child: Received from parent - '%s'\n", buffer);
        
        // Send second response
        char *child_msg2 = "I'm doing great, thanks!";
        printf("Child: Sending - '%s'\n", child_msg2);
        write(child_to_parent[1], child_msg2, strlen(child_msg2) + 1);
        
        // Close remaining pipe ends
        close(parent_to_child[0]);
        close(child_to_parent[1]);
        
        exit(EXIT_SUCCESS);
    }
    
    return 0;
}