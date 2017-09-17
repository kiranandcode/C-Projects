#ifndef THREAD_H
#define THREAD_H
#define T thread_T
struct T;
typedef struct T *T;

T       thread_new(void *(*func)(void *), void *data);
T       thread_detachednew(void *(*func)(void *), void *data);
void    thread_join(T thread);
void    thread_setreturn(T thread, void *);
void   *thread_getreturn(T thread);
void    thread_delete(T thread);
void    thread_globaldelete();

#undef T
#endif //THREAD_H
