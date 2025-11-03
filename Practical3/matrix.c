#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Function to initialize matrix with random values
void initialize_matrix(int **matrix, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

// Function for parallel matrix addition
void matrix_add(int **a, int **b, int **result, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

int main() {
    int sizes[] = {250, 500, 750, 1000, 2000};
    int threads[] = {1, 2, 4, 8};
    
    printf("Matrix Size,Threads,Time (s),Speedup\n");
    
    for (int s = 0; s < 5; s++) {
        int size = sizes[s];
        
        // Allocate matrices
        int **a = (int **)malloc(size * sizeof(int *));
        int **b = (int **)malloc(size * sizeof(int *));
        int **result = (int **)malloc(size * sizeof(int *));
        for (int i = 0; i < size; i++) {
            a[i] = (int *)malloc(size * sizeof(int));
            b[i] = (int *)malloc(size * sizeof(int));
            result[i] = (int *)malloc(size * sizeof(int));
        }
        
        // Initialize matrices
        initialize_matrix(a, size);
        initialize_matrix(b, size);
        
        double serial_time = 0;
        
        for (int t = 0; t < 4; t++) {
            omp_set_num_threads(threads[t]);
            double start = omp_get_wtime();
            
            matrix_add(a, b, result, size);
            
            double end = omp_get_wtime();
            double elapsed = end - start;
            
            if (threads[t] == 1) {
                serial_time = elapsed;
            }
            
            double speedup = serial_time / elapsed;
            printf("%d,%d,%.4f,%.2f\n", size, threads[t], elapsed, speedup);
        }
        
        // Free memory
        for (int i = 0; i < size; i++) {
            free(a[i]);
            free(b[i]);
            free(result[i]);
        }
        free(a);
        free(b);
        free(result);
    }
    
    return 0;
}