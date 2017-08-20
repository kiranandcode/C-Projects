#ifndef THREAD_H
#define THREAD_H

#define T Thread_T
typedef struct T *T;

extern int Thread_init(int preempt, ...);
extern int Thread_new(int apply(void *),
		void *args, int nbytes,...);
extern void Thread_exit(int code);
extern void Thread_alert(T t);
extern T Thread_self(void);
extern int Thread_join(T t);
extern void Thread_pause(void);

#endif
