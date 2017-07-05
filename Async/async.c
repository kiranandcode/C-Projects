#include <stdlib.h>
#include "assert.h"
#include "async.h"

#define T T_Task

struct T {
    int (*worker_function)(int state, void **result);
    int state;
};



T T_GenTask(int worker_function(int state, void **result)) {
    T task;

    task = malloc(sizeof(*task));

    task->worker_function = worker_function;
    task->state = 0;


    return task;
}




void T_Async_Run(T *tasks, void **results, int count) {

    assert(tasks);
    assert(results);
    assert(count > 0);

    int remaining = count;
    int position = 0;


    while(remaining > 0) {

       T task = tasks[position]; 
        // Check that the task has yet to finish
        if(task->state != -1) {
            
            int result_code = task->worker_function(task->state, results+position);
            

            assert(result_code == T_EXIT || result_code ==  T_ERR || result_code == T_CONT);


            // If an exit set state to end
            if(result_code == T_EXIT) {
                task->state = -1;
                remaining--;
            }  
            else if(result_code == T_ERR) {
                task->state = -1;
                *(results+position) = NULL;
                remaining--;
            } else {
                // increment the state
                task->state++;
            }

        }

        // move to the next position along
        position++;
        if(position >= count) position -= count;
    }

}



void T_DelTask(T task) {
    free(task);
}

#undef T
