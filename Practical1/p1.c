#include <stdio.h>
#include <omp.h>

int main()
{
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();   // Get thread ID
        printf("Hello, world from thread %d\n", tid);
    }
    return 0;
}
