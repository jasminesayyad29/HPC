#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int send_data = rank;
    int recv_data;
    if (rank == 0)
    {
        MPI_Ssend(&send_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Rank 0 sent data to Rank 1\n");
        MPI_Recv(&recv_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 0 received data %d from Rank 1\n", recv_data);
    }
    else if (rank == 1)
    {
        MPI_Ssend(&send_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Rank 1 sent data to Rank 0\n");
        MPI_Recv(&recv_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 1 received data %d from Rank 0\n", recv_data);
    }
    MPI_Finalize();
    return 0;
}
