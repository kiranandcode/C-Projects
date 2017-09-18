#include "thread.h"
#include <stdio.h>
#include <stdlib.h>

thread_mutex_T writing_mutex;
#define NUM_THREADS 20
THREAD_RETURN thr_func(void *arg) {
    thread_paramload(arg);
	printf("thread: %s\n", (char *) arg);
	int i;
    thread_mutexlock(writing_mutex);
    for(i = 0; i < 10; i++) {
	    printf("thread [%s]: %d\n", (char *)arg, i);

    }
    printf("remaining %d\n", 10-i);
    thread_mutexrelease(writing_mutex);

	thread_return(NULL);
}

int main() {
	writing_mutex = thread_mutexnew();
	thread_T threadA = thread_new(thr_func, "Test example 1");
	thread_T threadB = thread_new(thr_func, "Test example 2");
	thread_T threadC = thread_new(thr_func, "Test example 3");

	thread_join(threadA);
	thread_join(threadB);
	thread_join(threadC);

	thread_globaldelete();
	return EXIT_SUCCESS;
}
