// matrix_multiply_omp.c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
// gcc -fopenmp matrix.c -o matrix.exe
#define N 500  // Matrix size

void initialize_matrix(double **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void print_matrix(double **matrix, int size, char *name) {
    printf("%s:\n", name);
    for (int i = 0; i < (size < 5 ? size : 5); i++) {
        for (int j = 0; j < (size < 5 ? size : 5); j++) {
            printf("%6.2f ", matrix[i][j]);
        }
        printf("\n");
    }
    if (size > 5) printf("... (showing first 5 rows/columns)\n");
    printf("\n");
}

int main() {
    double **A, **B, **C_serial, **C_parallel;
    double start_time, end_time;
    int i, j, k;
    
    printf("=== Matrix Multiplication with OpenMP ===\n");
    printf("Matrix size: %d x %d\n", N, N);
    printf("Number of processors: %d\n", omp_get_num_procs());
    printf("Max threads: %d\n\n", omp_get_max_threads());
    
    // Allocate memory for matrices
    A = (double**)malloc(N * sizeof(double*));
    B = (double**)malloc(N * sizeof(double*));
    C_serial = (double**)malloc(N * sizeof(double*));
    C_parallel = (double**)malloc(N * sizeof(double*));
    
    for (i = 0; i < N; i++) {
        A[i] = (double*)malloc(N * sizeof(double));
        B[i] = (double*)malloc(N * sizeof(double));
        C_serial[i] = (double*)calloc(N, sizeof(double));
        C_parallel[i] = (double*)calloc(N, sizeof(double));
    }
    
    // Initialize matrices
    srand(time(NULL));
    initialize_matrix(A, N);
    initialize_matrix(B, N);
    
    printf("Matrices initialized.\n");
    print_matrix(A, N, "Matrix A");
    print_matrix(B, N, "Matrix B");
    
    // Serial matrix multiplication
    printf("Computing serial multiplication...\n");
    start_time = omp_get_wtime();
    
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C_serial[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    end_time = omp_get_wtime();
    printf("Serial time: %.4f seconds\n\n", end_time - start_time);
    
    // Parallel matrix multiplication
    printf("Computing parallel multiplication...\n");
    
    // Test different thread counts
    int thread_counts[] = {1, 2, 4, 8};
    
    for (int t = 0; t < 4; t++) {
        omp_set_num_threads(thread_counts[t]);
        
        // Reset result matrix
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                C_parallel[i][j] = 0;
            }
        }
        
        start_time = omp_get_wtime();
        
        #pragma omp parallel for private(i, j, k) shared(A, B, C_parallel)
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                double sum = 0;
                for (k = 0; k < N; k++) {
                    sum += A[i][k] * B[k][j];
                }
                C_parallel[i][j] = sum;
            }
        }
        
        end_time = omp_get_wtime();
        printf("Parallel with %d threads: %.4f seconds\n", 
               thread_counts[t], end_time - start_time);
    }
    
    print_matrix(C_parallel, N, "Result Matrix");
    
    // Verify results
    int correct = 1;
    for (i = 0; i < N && correct; i++) {
        for (j = 0; j < N && correct; j++) {
            if (abs(C_serial[i][j] - C_parallel[i][j]) > 0.001) {
                correct = 0;
                printf("Mismatch at [%d][%d]: serial=%.2f, parallel=%.2f\n", 
                       i, j, C_serial[i][j], C_parallel[i][j]);
            }
        }
    }
    
    if (correct) {
        printf("\n✓ Results verified: Parallel and serial results match!\n");
    }
    
    // Free memory
    for (i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C_serial[i]);
        free(C_parallel[i]);
    }
    free(A); free(B); free(C_serial); free(C_parallel);
    
    return 0;
}