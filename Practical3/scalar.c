#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter size of vectors: ");
    scanf("%d", &n);

    // Allocate memory
    int *a = (int*)malloc(n * sizeof(int));
    int *b = (int*)malloc(n * sizeof(int));
    int dot_product = 0;

    // Input vector a
    printf("Enter %d elements for vector a:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    // Input vector b
    printf("Enter %d elements for vector b:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &b[i]);
    }

    // Parallel dot product computation
    #pragma omp parallel for reduction(+:dot_product) schedule(static)
    for (int i = 0; i < n; i++) {
        dot_product += a[i] * b[i];
    }

    printf("Dot Product = %d\n", dot_product);

    free(a);
    free(b);
    return 0;
}