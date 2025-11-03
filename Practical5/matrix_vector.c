#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter the size of the square matrix (n): ");
    scanf("%d", &n);

    // Dynamic allocation
    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        matrix[i] = (int *)malloc(n * sizeof(int));

    int *vector = (int *)malloc(n * sizeof(int));
    int *result = (int *)malloc(n * sizeof(int));

    if (matrix == NULL || vector == NULL || result == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Random initialization
    for (int i = 0; i < n; i++) {
        vector[i] = rand() % 10;
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 10;
        }
    }

    // Initialize result
    for (int i = 0; i < n; i++)
        result[i] = 0;

    double start = omp_get_wtime();

    // Parallel matrix-vector multiplication
    #pragma omp parallel for shared(matrix, vector, result, n)
    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = 0; j < n; j++) {
            sum += matrix[i][j] * vector[j];
        }
        result[i] = sum;
    }

    double end = omp_get_wtime();

    if (n <= 5) {
        printf("\nMatrix:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                printf("%d ", matrix[i][j]);
            printf("\n");
        }

        printf("\nVector:\n");
        for (int i = 0; i < n; i++)
            printf("%d ", vector[i]);
        printf("\n");

        printf("\nResult (Matrix x Vector):\n");
        for (int i = 0; i < n; i++)
            printf("%d ", result[i]);
        printf("\n");
    }

    printf("\nExecution Time: %f seconds\n", end - start);

    // Free memory
    for (int i = 0; i < n; i++)
        free(matrix[i]);
    free(matrix);
    free(vector);
    free(result);

    return 0;
}
