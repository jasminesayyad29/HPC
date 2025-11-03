#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int M = 6;  // rows
    int N = 6;  // cols

    int *matrix = NULL;
    int *vector = (int*)malloc(N * sizeof(int));
    int *local_matrix = (int*)malloc((M/size) * N * sizeof(int));
    int *local_result = (int*)malloc((M/size) * sizeof(int));
    int *result = NULL;

    // Rank 0 initializes matrix and vector
    if (rank == 0) {
        matrix = (int*)malloc(M * N * sizeof(int));
        result = (int*)malloc(M * sizeof(int));

        printf("Matrix A:\n");
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i*N + j] = i + j + 1;  // simple values
                printf("%d ", matrix[i*N + j]);
            }
            printf("\n");
        }

        printf("Vector x:\n");
        for (int i = 0; i < N; i++) {
            vector[i] = i + 1;
            printf("%d ", vector[i]);
        }
        printf("\n");
    }

    // Broadcast vector to all processes
    MPI_Bcast(vector, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Scatter rows of matrix among processes
    MPI_Scatter(matrix, (M/size)*N, MPI_INT,
                local_matrix, (M/size)*N, MPI_INT,
                0, MPI_COMM_WORLD);

    // Each process computes its part of result
    for (int i = 0; i < M/size; i++) {
        local_result[i] = 0;
        for (int j = 0; j < N; j++) {
            local_result[i] += local_matrix[i*N + j] * vector[j];
        }
    }

    // Gather results back to root
    MPI_Gather(local_result, M/size, MPI_INT,
               result, M/size, MPI_INT,
               0, MPI_COMM_WORLD);

    // Rank 0 prints final result
    if (rank == 0) {
        printf("Result vector y = A * x:\n");
        for (int i = 0; i < M; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
    }

    free(local_matrix);
    free(local_result);
    free(vector);
    if (rank == 0) {
        free(matrix);
        free(result);
    }

    MPI_Finalize();
    return 0;
}
