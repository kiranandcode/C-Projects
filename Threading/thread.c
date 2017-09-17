#include "thread.h"
#include "sysinfo.h"
#include "list.h"

#include <stdlib.h>
#include <assert.h>

list_L list;
unsigned int is_initialized = 0;

static void initialize_threading() {
	if(!is_initialized) {
		list = list_new();
		is_initialized = 1;
	}
}

#if defined(SYSINFO_OS_LINUX)
// linux
#include <pthread.h>

struct thread_T {
	unsigned char isdetached;
	unsigned char isjoined;
	pthread_t thread;
	void *return_ptr;
};

thread_T       thread_new(void *(*func)(void *), void *data) {
	assert(func);
	initialize_threading();
	thread_T thread;
	thread = malloc(sizeof(*thread));

	assert(thread);

	thread->isdetached = 0;
	thread->isjoined = 0;
	thread->return_ptr = NULL;
	pthread_create(&thread->thread, NULL, func, data);

	// append to list
	list_push(list, thread);
	return thread;
}
thread_T       thread_detachednew(void *(*func)(void *), void *data) {

	initialize_threading();
	thread_T thread;
	thread = malloc(sizeof(*thread));
	thread->isdetached = 1;
	thread->return_ptr = NULL;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&thread->thread, &attr, func, data);

	pthread_attr_destroy(&attr);

	return thread;
}
void    thread_join(thread_T thread) {
	assert(!thread->isdetached);
	initialize_threading();
	if(!thread->isjoined) {
		pthread_join(thread->thread, thread->return_ptr);
		thread->isjoined = 1;
	}
}

void    thread_setreturn(thread_T thread, void *return_ptr) {
	assert(!thread->isdetached);
	initialize_threading();
	thread->return_ptr = return_ptr;
}

void   *thread_getreturn(thread_T thread) {
	assert(!thread->isdetached);
	initialize_threading();
	return thread->return_ptr;
}

void thread_delete(thread_T thread) {
	initialize_threading();
	if(!thread->isdetached && !thread->isjoined){ 
		thread_join(thread);
	}

	list_remove(list, thread);
	free(thread);
}

void thread_globaldelete() {
	initialize_threading();
	struct L_iterator iter = list_iterator(list);

	while(list_iteratorhasnext(&iter)) {
		thread_T thread = (thread_T) list_iteratornext(&iter);
		if(!thread->isdetached && !thread->isjoined){ 
			thread_join(thread);
		}
	}
}

#elif defined(SYSINFO_OS_WINDOWS)

#else
// error

#endif
