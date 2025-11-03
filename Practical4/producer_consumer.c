#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 10

int buffer[BUFFER_SIZE];
int count = 0;  // Number of items in buffer
int in = 0;     // Write index
int out = 0;    // Read index

void produce_item(int item) {
    printf("Produced: %d\n", item);
}

void consume_item(int item) {
    printf("Consumed: %d\n", item);
}

int main() {
    // Parallel region with all available threads
    #pragma omp parallel shared(buffer, count, in, out)
    {
        #pragma omp sections
        {
            // Producer section
            #pragma omp section
            {
                for (int i = 1; i <= PRODUCE_COUNT; i++) {
                    int item = i;

                    // Wait until buffer has space
                    while (count == BUFFER_SIZE) {
                        #pragma omp flush(count) // Ensure latest count is visible
                    }

                    #pragma omp critical
                    {
                        buffer[in] = item;
                        in = (in + 1) % BUFFER_SIZE;
                        count++;
                        produce_item(item);
                    }
                }
            }

            // Consumer section
            #pragma omp section
            {
                for (int i = 1; i <= PRODUCE_COUNT; i++) {
                    int item;

                    // Wait until buffer has data
                    while (count == 0) {
                        #pragma omp flush(count) // Ensure latest count is visible
                    }

                    #pragma omp critical
                    {
                        item = buffer[out];
                        out = (out + 1) % BUFFER_SIZE;
                        count--;
                        consume_item(item);
                    }
                }
            }
        }
    }

    return 0;
}
