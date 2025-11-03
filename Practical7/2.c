#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int n = 800; // matrix size (n x n), change for performance testing
    int rank, size;
    int *A, *B, *C;
    int *localA, *localC;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (n % size != 0) {
        if (rank == 0) printf("Matrix size must be divisible by processes.\n");
        MPI_Finalize();
        return 0;
    }

    int rows = n / size; 
    localA = (int*)malloc(rows * n * sizeof(int));
    localC = (int*)malloc(rows * n * sizeof(int));
    B = (int*)malloc(n * n * sizeof(int));

    if (rank == 0) {
        A = (int*)malloc(n * n * sizeof(int));
        C = (int*)malloc(n * n * sizeof(int));
        for (int i = 0; i < n * n; i++) {
            A[i] = 1;   // simple values
            B[i] = 1;   // all ones
        }
    }

    double start = MPI_Wtime();

    MPI_Scatter(A, rows * n, MPI_INT, localA, rows * n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    // compute local matrix multiplication
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += localA[i * n + k] * B[k * n + j];
            }
            localC[i * n + j] = sum;
        }
    }

    MPI_Gather(localC, rows * n, MPI_INT, C, rows * n, MPI_INT, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();
    double local_time = end - start;
    double max_time;

    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Matrix size: %d x %d\n", n, n);
        printf("Processes: %d\n", size);
        printf("Execution Time: %f seconds\n", max_time);

    }

    free(localA);
    free(localC);
    free(B);
    if (rank == 0) {
        free(A);
        free(C);
    }

    MPI_Finalize();
    return 0;
}
