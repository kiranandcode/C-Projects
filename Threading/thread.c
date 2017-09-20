#include "thread.h"
#include "sysinfo.h"
#include "list.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

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
#include <semaphore.h>

#include <unistd.h>

struct thread_T {
	unsigned char isdetached;
	unsigned char isjoined;
	pthread_t thread;
	void *return_ptr;
};

thread_T       thread_new(THREAD_RETURN (*func)(void *), void *data) {
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
	thread_return(thread);
}
thread_T       thread_detachednew(THREAD_RETURN (*func)(void *), void *data) {

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

	thread_return(thread);
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

void thread_sleep(unsigned int seconds) {
	sleep(seconds);
}

struct thread_mutex_T {
	pthread_mutex_t mutex;
	enum {
		MUTEX_DEFAULT = 0,
		MUTEX_RECURSIVE = 1,
		MUTEX_ERRCHECK = 2
	} mutex_type;
};

thread_mutex_T thread_mutexnew() {
	thread_mutex_T mutex;
	mutex = malloc(sizeof(*mutex));
	int res = pthread_mutex_init(&mutex->mutex, NULL);
	mutex->mutex_type = MUTEX_DEFAULT;
	if(res)
		assert(0);
	return mutex;
}
thread_mutex_T thread_mutexcustom(int mutex_type) {
	assert(mutex_type >= 0 && mutex_type < 3);
	thread_mutex_T mutex;
	mutex = malloc(sizeof(*mutex));

	int value;
	switch(mutex_type) {
		case 0:
			value = PTHREAD_MUTEX_TIMED_NP;
			break;
		case 1:
			value = PTHREAD_MUTEX_RECURSIVE_NP;
			break;
		case 2:
			value = PTHREAD_MUTEX_ERRORCHECK_NP;
			break;
	}
	pthread_mutexattr_t attr;
	int res = pthread_mutexattr_init(&attr);

	if(res)
		assert(0);
	pthread_mutexattr_settype(&attr,value);
	res = pthread_mutex_init(&mutex->mutex, &attr);

	pthread_mutexattr_destroy(&attr);
	
	mutex->mutex_type = mutex_type;
	if(res)
		assert(0);
	return mutex;
}
int thread_mutexlock(thread_mutex_T mutex) {
	return pthread_mutex_lock(&mutex->mutex);
}
int thread_mutextrylock(thread_mutex_T mutex) {
	return pthread_mutex_trylock(&mutex->mutex);
}
int thread_mutexrelease(thread_mutex_T mutex) {
	return	pthread_mutex_unlock(&mutex->mutex);
}

void thread_mutexdelete(thread_mutex_T mutex) {
	assert(mutex);
	int res = pthread_mutex_destroy(&mutex->mutex);
	if(res)
		assert(0);
	free(mutex);
}

struct thread_semaphore_T {
	sem_t semaphore;
};
thread_semaphore_T  thread_semaphorenew(int value) {
	thread_semaphore_T semaphore;
	semaphore = malloc(sizeof(*semaphore));
	assert(semaphore);
	int res = sem_init(&semaphore->semaphore, 0, value);
	if(res)
		assert(0);
	return semaphore;
}
int thread_semaphoreclaim(thread_semaphore_T semaphore) {
	return sem_wait(&semaphore->semaphore);
}
int thread_semaphorerelease(thread_semaphore_T semaphore) {
	return sem_post(&semaphore->semaphore);
}
void thread_semaphoredelete(thread_semaphore_T semaphore) {
	assert(semaphore);
	int res = sem_destroy(&semaphore->semaphore);
	if(res) 
		assert(0);
	free(semaphore);
}



#elif defined(SYSINFO_OS_WINDOWS)
#include <windows.h>
#include <process.h>
struct thread_T {
	unsigned char isjoined;
    unsigned char isdetached;
	HANDLE process_handle;
	void *returnptr;
};

thread_T       thread_new(THREAD_RETURN (*func)(void *), void *data) {
    assert(func);
	initialize_threading();
	thread_T thread;
	thread = malloc(sizeof(*thread));

	thread->isjoined = 0;
	thread->isdetached = 0;
	thread->returnptr = data;
	thread->process_handle = (HANDLE) _beginthread(func, 0, thread);

	list_push(list, thread);

    return thread;
}
thread_T       thread_detachednew(THREAD_RETURN (*func)(void *), void *data) {
	assert(func);
	initialize_threading();
	thread_T thread = thread_new(func, data);
	thread->isdetached = 1;

	return thread;
}
void    thread_join(thread_T thread) {
	assert(thread);
	assert(!thread->isdetached);
	initialize_threading();
	if(!thread->isdetached && !thread->isjoined) {
		WaitForSingleObject(thread->process_handle, INFINITE);
        thread->isjoined = 1;
	}
}

void    thread_setreturn(thread_T thread, void * returnptr) {
	assert(thread);
	assert(!thread->isjoined);
	initialize_threading();
	if(thread->isdetached) {
		thread->returnptr = NULL;
	}
	else
        thread->returnptr = returnptr;
}
void   *thread_getreturn(thread_T thread) {
	assert(thread);
	initialize_threading();
	return thread->returnptr;
}

void    thread_delete(thread_T thread) {
	assert(thread);
	initialize_threading();
	if(!thread->isjoined && !thread->isdetached) {
		thread_join(thread);
	}
	list_remove(list, thread);
	free(thread);
}
void    thread_globaldelete() {
	initialize_threading();
	struct L_iterator iter = list_iterator(list);

	while(list_iteratorhasnext(&iter)) {
		thread_T thread = (thread_T) list_iteratornext(&iter);
		if(!thread->isdetached && !thread->isjoined){
			thread_join(thread);
		}
	}
}
void thread_sleep(unsigned int seconds) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
	return;
}


struct thread_mutex_T {
};

thread_mutex_T thread_mutexnew();
thread_mutex_T thread_mutexnewcustom(int mutex_type);
int thread_mutexlock(thread_mutex_T mutex);
int thread_mutextrylock(thread_mutex_T mutex);
int thread_mutexrelease(thread_mutex_T mutex);

thread_semaphore_T  thread_semaphorenew();
int thread_semaphoreclaim(thread_semaphore_T semaphore);
int thread_semaphorerelease(thread_semaphore_T semaphore);



#else
// error

thread_T       thread_new(THREAD_RETURN (*func)(void *), void *data) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
}
thread_T       thread_detachednew(void *(*func)(void *), void *data) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
}
void    thread_join(thread_T thread) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
}
void    thread_setreturn(thread_T thread, void *) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
}
void   *thread_getreturn(thread_T thread){
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
}
void    thread_delete(thread_T thread) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
}
void    thread_globaldelete() {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
}


void thread_sleep(unsigned int seconds) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
	return;
}

thread_mutex_T thread_mutexnew() {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
	return;
}
thread_mutex_T thread_mutexnewcustom(int mutex_type) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
	return;
}
int thread_mutexlock(thread_mutex_T mutex) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
	return;
}

int thread_mutextrylock(thread_mutex_T mutex) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
	return;
}

int thread_mutexrelease(thread_mutex_T mutex) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
	return;
}

thread_semaphore_T  thread_semaphorenew() {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
	return NULL;
}
int thread_semaphoreclaim(thread_semaphore_T semaphore) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
	return;
}
int thread_semaphorerelease(thread_semaphore_T semaphore) {
	fprintf(stderr, "THREADERROR: Platform not supported.");
	assert(0);
	return;
}



#endif
