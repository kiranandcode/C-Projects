#ifndef JOBQUEUE_H
#define JOBQUEUE_H
#define T jobqueue_T
struct T;
typedef struct T *T;

T jobqueue_new(void (*process_func)(void *));
void jobqueue_delete(T queue);
void jobqueue_enqueue(T queue, void *data);
void jobqueue_cancel(T queue);

#undef T
#endif
