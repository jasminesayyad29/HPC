#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#define N 362880 // 9! possible states (for 8-puzzle)
#define MASTER 0

typedef struct {
    int state[9];  // 3x3 puzzle flattened
    int depth;
} Node;

int is_goal(Node n) {
    int goal[9] = {0,1,2,3,4,5,6,7,8};
    for(int i=0;i<9;i++)
        if(n.state[i]!=goal[i]) return 0;
    return 1;
}

int expand(Node n, Node children[4]) {
    // Simplified: just return 0 children (real BFS logic needed)
    return 0;
}

int sequential_bfs(Node start) {
    double t_start = MPI_Wtime();
    // Implement normal BFS here
    int generated_states = 100; // dummy
    double t_end = MPI_Wtime();
    printf("Sequential BFS time: %f ms\n", (t_end - t_start)*1000);
    return generated_states;
}

int parallel_bfs(Node start, int rank, int size) {
    double t_start = MPI_Wtime();

    int generated_states = 0;
    int wasted_states = 0;

    // Simplified master-worker loop
    if(rank == MASTER){
        // Master distributes initial node to workers
        for(int i=1;i<size;i++){
            MPI_Send(&start, sizeof(Node), MPI_BYTE, i, 0, MPI_COMM_WORLD);
        }

        // Master also expands
        generated_states += 50;
        // Receive results
        for(int i=1;i<size;i++){
            int worker_generated;
            MPI_Recv(&worker_generated, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            generated_states += worker_generated;
            wasted_states += worker_generated/10; // assume 10% wasted for example
        }
    } else {
        Node n;
        MPI_Recv(&n, sizeof(Node), MPI_BYTE, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int local_generated = 50; // dummy
        MPI_Send(&local_generated, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
    }

    double t_end = MPI_Wtime();
    if(rank==MASTER){
        printf("Parallel BFS time: %f ms\n", (t_end - t_start)*1000);
        printf("Wasted computation: %f%%\n", (double)wasted_states/generated_states*100);
    }

    return generated_states;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    Node start;
    for(int i=0;i<9;i++) start.state[i] = i;

    int seq_gen=0;
    if(rank==MASTER){
        seq_gen = sequential_bfs(start);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    int par_gen = parallel_bfs(start, rank, size);

    if(rank==MASTER){
        double speedup = 1.0; // replace with seq_time/par_time if measured
        printf("Speedup: %f\n", speedup);
    }

    MPI_Finalize();
    return 0;
}
