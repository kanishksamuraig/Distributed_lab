// parallel_sum.c
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100

int main(int argc, char *argv[]) {
    int rank, size;
    int *array = NULL;
    int local_sum = 0, global_sum = 0;
    int elements_per_process;
    int *local_array;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    elements_per_process = ARRAY_SIZE / size;
    local_array = (int*)malloc(elements_per_process * sizeof(int));
    
    if (rank == 0) {
        // Process 0 creates and initializes the array
        array = (int*)malloc(ARRAY_SIZE * sizeof(int));
        srand(time(NULL));
        
        printf("Array elements: ");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = rand() % 10;
            printf("%d ", array[i]);
        }
        printf("\n\n");
    }
    
    // Distribute array to all processes
    MPI_Scatter(array, elements_per_process, MPI_INT, 
                local_array, elements_per_process, MPI_INT, 
                0, MPI_COMM_WORLD);
    
    // Each process calculates its local sum
    printf("Process %d: ", rank);
    for (int i = 0; i < elements_per_process; i++) {
        local_sum += local_array[i];
        printf("%d ", local_array[i]);
    }
    printf("- Local sum: %d\n", local_sum);
    
    // Gather all local sums to process 0
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("\nGlobal sum = %d\n", global_sum);
        free(array);
    }
    
    free(local_array);
    MPI_Finalize();
    return 0;
}