#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0
#define NUM_PROCESSES 5
#define TIME_QUANTUM 2  // for RR

typedef struct {
    int pid;
    int burst_time;
    int arrival_time;
} Process;

// Function to simulate Round-Robin
int simulate_RR(Process p[], int n) {
    int time = 0;
    int remaining[NUM_PROCESSES];
    for(int i=0;i<n;i++) remaining[i] = p[i].burst_time;

    int completed = 0;
    int total_time = 0;
    while(completed < n){
        for(int i=0;i<n;i++){
            if(remaining[i] > 0){
                int t = remaining[i] < TIME_QUANTUM ? remaining[i] : TIME_QUANTUM;
                remaining[i] -= t;
                time += t;
                total_time += time;
                if(remaining[i]==0) completed++;
            }
        }
    }
    return total_time;  // total CPU time for simplicity
}

// Function to simulate Shortest Job First
int simulate_SJF(Process p[], int n) {
    int completed = 0;
    int time = 0;
    int remaining[NUM_PROCESSES];
    for(int i=0;i<n;i++) remaining[i] = p[i].burst_time;

    int total_time = 0;
    while(completed < n){
        int idx = -1;
        int min_burst = 1e9;
        for(int i=0;i<n;i++){
            if(remaining[i]>0 && p[i].arrival_time <= time && remaining[i]<min_burst){
                min_burst = remaining[i];
                idx = i;
            }
        }
        if(idx == -1){ time++; continue; }
        time += remaining[idx];
        total_time += time;
        remaining[idx] = 0;
        completed++;
    }
    return total_time;
}

int main(int argc, char** argv){
    MPI_Init(&argc,&argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    // Example processes
    Process proc[NUM_PROCESSES] = {
        {0,5,0},{1,3,0},{2,8,0},{3,6,0},{4,2,0}
    };

    double t_start, t_end;
    int local_result;

    t_start = MPI_Wtime();

    if(rank == 0) {
        local_result = simulate_RR(proc, NUM_PROCESSES);
    } else if(rank == 1) {
        local_result = simulate_SJF(proc, NUM_PROCESSES);
    } else {
        local_result = 0; // extra ranks do nothing
    }

    int results[2] = {0,0};
    MPI_Gather(&local_result,1,MPI_INT,results,1,MPI_INT,MASTER,MPI_COMM_WORLD);

    t_end = MPI_Wtime();

    if(rank == MASTER){
        int chosen_result = results[1]; // choose SJF as final result
        double wasted = 0;
        if(results[0]!=0) wasted = 100.0; // RR work discarded

        printf("Sequential times (ms) - RR dummy: %d, SJF dummy: %d\n", results[0], results[1]);
        printf("Parallel time (ms): %f\n", (t_end - t_start)*1000);
        double speedup = 1.0; // can compute by running sequentially both
        printf("Wasted computation: %.2f%%\n", wasted);
        printf("Chosen scheduling policy result (SJF): %d\n", chosen_result);
    }

    MPI_Finalize();
    return 0;
}
