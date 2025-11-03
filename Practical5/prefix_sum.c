#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter the size of the array: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    int *prefix = (int *)malloc(n * sizeof(int));

    // Initialize array with random numbers
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100;  // values between 0–99
    }

    double start = omp_get_wtime();

    // Prefix Sum (Sequential part: scan step-by-step)
    prefix[0] = arr[0];

    #pragma omp parallel for shared(arr, prefix, n)
    for (int i = 1; i < n; i++) {
        prefix[i] = prefix[i - 1] + arr[i];
    }

    double end = omp_get_wtime();
    if(n <= 100) {
        printf("\nPrefix Sum Array:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", prefix[i]);
        }
        printf("\n");
    }   

    printf("\nTime taken: %f seconds\n", end - start);

    free(arr);
    free(prefix);
    return 0;
}
