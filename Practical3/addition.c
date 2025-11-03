#include <stdio.h>
#include <omp.h>

#define SIZE 2000

int main() {
    int vector[SIZE], scalar = 5;
    double start_time, end_time;

    for (int i = 0; i < SIZE; i++) {
        vector[i] = i;
    }

    printf("STATIC Scheduling:\n");
    for (int chunk = 10; chunk <= 50; chunk += 20) {
        start_time = omp_get_wtime();
        #pragma omp parallel for schedule(static, chunk)
        for (int i = 0; i < SIZE; i++) {
            vector[i] += scalar;
        }
        end_time = omp_get_wtime();
        printf("Chunk=%d, Time=%.6f sec\n", chunk, end_time - start_time);
    }

    printf("\nDYNAMIC Scheduling:\n");
    for (int chunk = 10; chunk <= 50; chunk += 20) {
        start_time = omp_get_wtime();
        #pragma omp parallel for schedule(dynamic, chunk)
        for (int i = 0; i < SIZE; i++) {
            vector[i] += scalar;
        }
        end_time = omp_get_wtime();
        printf("Chunk=%d, Time=%.6f sec\n", chunk, end_time - start_time);
    }

    printf("\nNOWAIT Clause:\n");
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int i = 0; i < SIZE; i++) {
            vector[i] += scalar;
        }
        #pragma omp barrier  
    }
    end_time = omp_get_wtime();
    printf("Time with nowait=%.6f sec\n", end_time - start_time);

    return 0;
}