#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAYSIZE 100
#define NUMTHREADS 100

// Pthread library only allows the passing of one argument to a thread, so we
// have to wrap our arguments in a struct
struct ThreadData
{
    int start, stop;
    int *array;
};

int *before(void);
void oldApproach(void);
void *justLoop(struct ThreadData *td);
void newApproach(void);





int main()
{


    newApproach();

    return 1;


}


int *before(void)
{

    int *output = malloc(sizeof(int)*ARRAYSIZE);
    int i;
    for(i=0; i<ARRAYSIZE; i++)
    {
        output[i] = i*i;
    }
    return output;

}

void oldApproach(void)
{
    //Run function to generate all the values linearly
    int *cool = before();

    //Print all the values - needs to be in order, because I am a human.
    int i;
    for(i = 0; i < ARRAYSIZE; i++) printf("[%d]: %d\n", i, cool[i]);

    free(cool);
    return;
}

void *justLoop(struct ThreadData *td)
{
    int i;

    for(i=td->start; i<td->stop; i++) td->array[i] = i*i;

    return NULL;
}

void newApproach(void)
{
    // Declare the array prior to use - rather than creating it in the function.
    int squares[ARRAYSIZE];

    // Create an array of arguments for each thread
    struct ThreadData data [NUMTHREADS];

    //Create threads to hold all threads
    pthread_t thread[NUMTHREADS];


    // Just more tasks per thread than we need - quickest way of achieving result.
    int tasksPerThread =  (ARRAYSIZE+NUMTHREADS-1)/NUMTHREADS;

    int i, start, stop;


    // Create loop to separate up tasks into their own structs
    for (i = 0; i < NUMTHREADS; i++)
    {
        data[i].start = i*tasksPerThread;
        data[i].stop = (i+1)*tasksPerThread;
        data[i].array = squares;
    }

    data[NUMTHREADS-1].stop = ARRAYSIZE;

    // Loop and begin the threads
    for (i = 0; i < NUMTHREADS; i++) pthread_create(thread+i, NULL, justLoop, data+i);


    // Wait for threads to end
    for(i=0; i<NUMTHREADS; i++) pthread_join(thread+i, NULL);


    for(i = 0; i < ARRAYSIZE; i++) printf("[%d]: %d\n", i, squares[i]);
    return;

}


// Summary of findings from this experiment
// To implement threading in a C program - the following steps must be done:
//
// Create function to perform small unconnected sub section of the main problem.
// Create container for results
// Create container for threads
// Create container for arguments
// Generate arguments for data
// Create the threads - pthread create(thread container,  NULL, function, argument container)
// Join each of the threads - pthread join(thread container, NULL)
// Done
