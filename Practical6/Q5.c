#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char **argv)
{
    const int n = 8;
    int A[n];
    int local_sum = 0, total_sum = 0;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 2)
    {
        if (rank == 0)
            printf("This program requires exactly 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        srand(time(NULL));
        printf("Full array A: ");
        for (int i = 0; i < n; i++)
        {
            A[i] = rand() % 100;
            printf("%d ", A[i]);
        }
        printf("\n");
    }

    MPI_Bcast(A, n, MPI_INT, 0, MPI_COMM_WORLD);

    int start = (rank == 0) ? 0 : n / 2;
    int end = (rank == 0) ? n / 2 : n;

    for (int i = start; i < end; i++)
    {
        local_sum += A[i];
    }

    printf("Process %d computed partial sum = %d\n", rank, local_sum);

    if (rank == 0)
    {
        int remote_sum;
        MPI_Recv(&remote_sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total_sum = local_sum + remote_sum;
        printf("Final total sum = %d\n", total_sum);
    }
    else if (rank == 1)
    {
        MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
