#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

#define NUM_STEPS 100000

int main() {
    double pi_estimate = 0.0;
    double step = 1.0 / NUM_STEPS; //width
    double x, sum = 0.0; // height of each strip
    double start_time, end_time;

    int num_threads;
    printf("Enter number of threads: ");
    scanf("%d", &num_threads);

    start_time = omp_get_wtime();

    // Parallel computation using OpenMP
    #pragma omp parallel for private(x) reduction(+:sum) num_threads(num_threads)
    for (int i = 0; i < NUM_STEPS; i++) {
        x = (i + 0.5) * step;  // Midpoint of the interval
        sum += 4.0 / (1.0 + x * x);  //  4/(1+x²)
    }

    pi_estimate = step * sum;  

    end_time = omp_get_wtime();

    printf("Estimated Pi = %.16f\n", pi_estimate);
    printf("Actual Pi    = 3.141592653589793\n");
    printf("Error        = %.16f\n", fabs(pi_estimate - 3.141592653589793));
    printf("Time taken   = %.6f seconds\n", end_time - start_time);

    return 0;
}