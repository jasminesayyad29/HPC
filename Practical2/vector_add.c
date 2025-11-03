#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;
    float scalar;
    
    printf("Enter size of vector: ");
    scanf("%d", &n);
    
    printf("Enter scalar value: ");
    scanf("%f", &scalar);

    float *vector = (float *)malloc(n * sizeof(float));
    float *result = (float *)malloc(n * sizeof(float));
    if (!vector || !result) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Initialize vector
    for (int i = 0; i < n; i++) {
        vector[i] = i * 1.5f;
    }

    // Print first 10 input values
    printf("\nFirst 10 input values:\n");
    for (int i = 0; i < (n < 10 ? n : 10); i++) {
        printf("vector[%d] = %.2f\n", i, vector[i]);
    }

    int threads;
    printf("\nEnter number of threads: ");
    scanf("%d", &threads);

    double start_time = omp_get_wtime();

    omp_set_num_threads(threads);
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        result[i] = vector[i] + scalar;
    }

    double end_time = omp_get_wtime();

    printf("\nFirst 10 results:\n");
    for (int i = 0; i < (n < 10 ? n : 10); i++) {
        printf("result[%d] = %.2f\n", i, result[i]);
    }

    printf("\nExecution time: %f seconds\n", end_time - start_time);

    free(vector);
    free(result);
    return 0;
}