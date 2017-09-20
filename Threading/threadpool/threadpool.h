#ifndef THREADPOOL_H
#define THREADPOOL_H
#define T threadpool_T
struct T;
typedef struct T *T;

void threadpool_enqueue(T threadpool, void (*func) (void *), void * data);
void threadpool_cancel(T threadpool);
T threadpool_new(unsigned int thread_count);
void threadpool_delete(T threadpool);
void threadpool_join(T threadpool);

#undef T
#endif //THREADPOOL_H
