#ifndef ASYNC_H
#define ASYCN_H

#define T T_Task

// Exit codes for worker functions
#define T_EXIT 1
#define T_ERR -1
#define T_CONT 0

typedef struct T *T;

T    T_GenTask(int worker_function(int state, void **result));
void T_DelTask(T task);
void T_Async_Run(T *tasks, void **results, int count);

#endif
