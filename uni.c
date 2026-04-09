#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    // Array to hold the read and write file descriptors
    int fd[2];
    pid_t pid;

    char write_msg[] = "Hello from the Parent process! Data flows one way.";
    char read_buffer[100];

    // 1. Create the pipe BEFORE forking
    // pipe(fd) populates fd[0] for reading and fd[1] for writing
    if (pipe(fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Fork the process to create a child
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // ---------------------------------------------------------
    // PARENT PROCESS LOGIC (The Writer)
    // ---------------------------------------------------------
    if (pid > 0) {
        // Best Practice: Close the unused read end in the parent
        close(fd[0]);

        printf("Parent [PID %d]: Writing message to the pipe...\n", getpid());

        // Write the message to fd[1]. We add +1 to include the null terminator '\0'
        write(fd[1], write_msg, strlen(write_msg) + 1);

        // Close the write end after sending data to signal EOF to the reader
        close(fd[1]);

        // Wait for the child to finish executing
        wait(NULL);
    }
    // ---------------------------------------------------------
    // CHILD PROCESS LOGIC (The Reader)
    // ---------------------------------------------------------
    else {
        // Best Practice: Close the unused write end in the child
        close(fd[1]);

        printf("Child  [PID %d]: Reading from the pipe...\n", getpid());

        // Read data from fd[0] into the buffer
        read(fd[0], read_buffer, sizeof(read_buffer));

        printf("Child  [PID %d]: Received message: '%s'\n", getpid(), read_buffer);

        // Close the read end when done
        close(fd[0]);
    }

    return 0;
}