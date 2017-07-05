#include <stdio.h>
#include <stdlib.h>
#include "async.c"





// Example async code
int async_print_to_20(int state, void **result) {
    
    // Let's say that printing to a 20 is a task that takes too long and will block.
    // to reduce the load, we're gonna split it up into 5 smaller tasks - allowing other
    // functions to run in the meantime
    
   // the state variable passed in each time acts as a good way of maintaining state.
   // However if it doesn't provide the information you need you can always use static
   // variables -------- or even BETTER use the void * as a temporary storage
   
   int i; 

    switch(state) {

        case 0:
            i = 0;
            for(; i<4; i++) printf("Printing to 20: %d\n", i);
            return T_CONT;
            break;
        case 1:
            i = 4;
            for(; i<8; i++) printf("Printing to 20: %d\n", i);
            return T_CONT;
            break;
        case 2:
            i = 8;
            for(; i<12; i++) printf("Printing to 20: %d\n", i);
            return T_CONT;
            break;
        case 3:
            i = 12;
            for(; i<16; i++) printf("Printing to 20: %d\n", i);
            return T_CONT;
            break;
        case 4:
            // Final task. Here we go!!!
            i = 16;
            for(; i<20; i++) printf("Printing to 20: %d\n", i);
            *(result) = NULL;
            return T_EXIT;
            break;
        default:
            // Shouldn't ever enter this loop
            // Mega error.
            return T_ERR;
           break; 
    }


}

int async_fibbonacci_number(int state, void **result) {
    // Now let's look at a function that will have to return a result
    // This time we'll need more complex state than can be provided by
    // just the state alone.
    // Thus we expand our horizons.
    // We will use the void * given to store the result as a temporary storage for our session variables
    

    struct storage {
        int nth;
        int storage[2];
    };

    struct storage *s;
    switch(state) {

        case 0:
            // In the initial state, we set up the local variables
            s = malloc(sizeof(*s));
            s->nth = 0;
            s->storage[0] = 0;
            s->storage[1] = 1;

            *result = (void *) s;

            for(;s->nth < 10; s->nth++) {
                int temp = s->storage[0] + s->storage[1];
                s->storage[0] = s->storage[1];
                s->storage[1] = temp;
            }

            return T_CONT;
            break;
        case 1:
            s = (struct storage *) *result;

            for(;s->nth < 20; s->nth++) {
                int temp = s->storage[0] + s->storage[1];
                s->storage[0] = s->storage[1];
                s->storage[1] = temp;
            }

            return T_CONT;
            break;
        case 2:

            s = (struct storage *) *result;

            for(;s->nth < 30; s->nth++) {
                int temp = s->storage[0] + s->storage[1];
                s->storage[0] = s->storage[1];
                s->storage[1] = temp;
            }

            return T_CONT;
            break;
        case 3:
            // our final run. Let's do this!!!
            s = (struct storage *) *result;

            for(;s->nth < 40; s->nth++) {
                int temp = s->storage[0] + s->storage[1];
                s->storage[0] = s->storage[1];
                s->storage[1] = temp;
            }
            
            // Calculated the result - it is in storage[1];
            int value = s->storage[1];

            // delete our temporary storage
            free(s);

            // set up a location to store the result
            int *store = malloc(sizeof(int));
            *store = value;
            *result = store;


            // for the purposes of visualisng the function:
            printf("completed fibnc\n");
            return T_EXIT;
            break;

        default:
            // shouldn't be here
            return T_ERR;
            break;

    }


}

int main() {


    T_Task io_task = T_GenTask(async_print_to_20);
    T_Task comp_task = T_GenTask(async_fibbonacci_number);
    
    T_Task *tasks = malloc(sizeof(T_Task)*2);

    tasks[0] = io_task;
    tasks[1] = comp_task;

    void **results = calloc(sizeof(void *),2);

    T_Async_Run(tasks, results, 2);

    T_DelTask(io_task);
    T_DelTask(comp_task);

    free(tasks);

    printf("Fibb result: %d\n", *(int *)results[1]);
   
    free(results[1]);
    free(results);
}
