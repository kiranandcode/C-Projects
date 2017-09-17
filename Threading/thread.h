#ifndef THREAD_H
#define THREAD_H

#include "sysinfo.h"

#ifdef SYSINFO_OS_LINUX
#define THREAD_RETURN void *
#define thread_return(a) (return (void *)a)
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
struct T;
typedef struct T *T;

T       thread_new(THREAD_RETURN (*func)(void *), void *data);
T       thread_detachednew(THREAD_RETURN (*func)(void *), void *data);
void    thread_join(T thread);
void    thread_setreturn(T thread, void *);
void   *thread_getreturn(T thread);
void    thread_delete(T thread);
void    thread_globaldelete();

#undef T
#endif //THREAD_H