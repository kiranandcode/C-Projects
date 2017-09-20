#ifndef THREAD_H
#define THREAD_H

#include "sysinfo.h"

#ifdef SYSINFO_OS_LINUX
#define THREAD_RETURN void *
#define thread_return(a) do {return (void *)a;} while(0)
#define thread_paramload(a) 0
#elif defined(SYSINFO_OS_WINDOWS)
#define THREAD_RETURN void
#define thread_return(a) do { thread_setreturn(_t_data, a); return; } while(0);
#define thread_paramload(a) thread_T _t_data; do {_t_data = (thread_T) a; a = thread_getreturn(_t_data); } while(0)
#else
#define THREAD_RETURN void
#define thread_return(a) do { fprintf("THREAD_ERR: Unsupported Platform"); assert(0); } while(0)
#define thread_paramload(a) do { fprintf("THREAD_ERR: Unsupported Platform"); assert(0); } while(0)
#endif

#define T thread_T
#define M thread_mutex_T
#define S thread_semaphore_T
struct T;
typedef struct T *T;
struct M;
typedef struct M *M;
struct S;
typedef struct S *S;

T       thread_new(THREAD_RETURN (*func)(void *), void *data);
T       thread_detachednew(THREAD_RETURN (*func)(void *), void *data);
void    thread_join(T thread);
void    thread_setreturn(T thread, void *);
void   *thread_getreturn(T thread);
void    thread_delete(T thread);
void    thread_globaldelete();
void    thread_sleep(unsigned int seconds);

M       thread_mutexnew();
M       thread_mutexnewcustom(int mutex_type);
int     thread_mutexlock(M mutex);
int     thread_mutextrylock(M mutex);
int     thread_mutexrelease(M mutex);
void thread_mutexdelete(M mutex);

S       thread_semaphorenew();
int     thread_semaphoreclaim(S semaphore);
int     thread_semaphorerelease(S semaphore);
void thread_semaphoredelete(S semaphore);

#undef T
#endif //THREAD_H
