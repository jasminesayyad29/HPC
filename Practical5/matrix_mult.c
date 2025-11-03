#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter the size of the square matrices: ");
    scanf("%d", &n);

    // Dynamic allocation for matrices
    int **a = (int **)malloc(n * sizeof(int *));
    int **b = (int **)malloc(n * sizeof(int *));
    int **c = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        a[i] = (int *)malloc(n * sizeof(int));
        b[i] = (int *)malloc(n * sizeof(int));
        c[i] = (int *)malloc(n * sizeof(int));
    }

    // Initialize matrices with random values
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = rand() % 10;
            b[i][j] = rand() % 10;
            c[i][j] = 0;
        }
    }

    double start = omp_get_wtime();

    // Parallel matrix multiplication
    #pragma omp parallel for collapse(2) shared(a, b, c, n)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }

    double end = omp_get_wtime();

    // Print result matrix (only for small n)
    if (n <= 10) {
        printf("\nMatrix A:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                printf("%3d ", a[i][j]);
            printf("\n");
        }

        printf("\nMatrix B:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                printf("%3d ", b[i][j]);
            printf("\n");
        }

        printf("\nResult Matrix C = A * B:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                printf("%3d ", c[i][j]);
            printf("\n");
        }
    }

    printf("\nTime taken for multiplication: %f seconds\n", end - start);

    // Free memory
    for (int i = 0; i < n; i++) {
        free(a[i]);
        free(b[i]);
        free(c[i]);
    }
    free(a);
    free(b);
    free(c);

    return 0;
}
// Compile with: gcc -fopenmp -o matrix_mult matrix_mult.c
// Run with: ./matrix_mult