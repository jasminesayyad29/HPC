#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, scalar;
    printf("Enter the size of the square matrix: ");
    scanf("%d", &n);

    printf("Enter the scalar value: ");
    scanf("%d", &scalar);

    // Allocate memory dynamically on heap
    int *a = (int *)malloc(n * n * sizeof(int));
    int *c = (int *)malloc(n * n * sizeof(int));

    if (a == NULL || c == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Initialize matrix with simple values
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = i + j;  
        }
    }

    double start = omp_get_wtime();

    // Parallel scalar multiplication
    #pragma omp parallel for collapse(2) shared(a, c, scalar, n)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i * n + j] = scalar * a[i * n + j];
        }
    }

    double end = omp_get_wtime();

    // Print result only for small n
    if (n <= 5) {
        printf("\nOriginal Matrix (A):\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%5d ", a[i * n + j]);
            }
            printf("\n");
        }

        printf("\nResult Matrix (C = %d * A):\n", scalar);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%5d ", c[i * n + j]);
            }
            printf("\n");
        }
    }

    printf("\nTime taken: %f seconds\n", end - start);

    free(a);
    free(c);
    return 0;
}
