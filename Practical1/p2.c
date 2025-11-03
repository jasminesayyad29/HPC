#include <stdio.h>
#include <omp.h>

int main() {
    int n;

    printf("Enter the number of threads: ");
    scanf("%d", &n);

    // Sequential printing
    printf("\nSequential printing:\n");
    for (int i = 0; i < n; i++) {
        printf("Hello, World from thread %d (sequential)\n", i);
    }

    // Parallel printing
    printf("\nParallel printing:\n");
    omp_set_num_threads(n);  
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        printf("Hello, World from thread %d (parallel)\n", thread_id);
    }

    return 0;
}
