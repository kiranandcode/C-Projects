#include "jobqueue.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "thread.h"

struct jobqueue_T {
	unsigned char iscancelled:1;
	unsigned char isstopped:1;

	thread_semaphore_T jobsemaphore;
	thread_mutex_T     cancelmutex;
	thread_mutex_T     stopmutex;
	thread_mutex_T     queuemutex;

	void (*process_func)(void *);
	struct jobqueue_elem_T *job_queue;
	thread_T executer_thread;
};

struct jobqueue_elem_T {
	void *data;
	struct jobqueue_elem_T *next;
};

THREAD_RETURN jobqueue_executor(void *input);


jobqueue_T jobqueue_new(void (*process_func)(void *)) {
	assert(process_func);
	jobqueue_T queue;
	queue = malloc(sizeof(*queue));
	assert(queue);
	queue->iscancelled = 0;
	queue->isstopped = 0;

	queue->process_func = process_func;
	queue->job_queue = NULL;
	queue->jobsemaphore = thread_semaphorenew(0);
	queue->cancelmutex  = thread_mutexnew();
	queue->stopmutex  = thread_mutexnew();
	queue->queuemutex  = thread_mutexnew();

	queue->executer_thread = thread_new(jobqueue_executor, queue);
	return queue;
}

void jobqueue_delete(jobqueue_T queue) {
	assert(queue);

	int needs_stopping = 0;
	thread_mutexlock(queue->stopmutex);
	if(!queue->isstopped) {
		needs_stopping = 1;
	}
	thread_mutexrelease(queue->stopmutex);

	if(needs_stopping)
		jobqueue_cancel(queue);

	thread_mutexdelete(queue->queuemutex);
	thread_mutexdelete(queue->stopmutex);
	thread_mutexdelete(queue->cancelmutex);
	thread_semaphoredelete(queue->jobsemaphore);
	thread_delete(queue->executer_thread);


	struct jobqueue_elem_T *elem,*next;
	elem = queue->job_queue;
	while(elem != NULL) {
		next = elem->next;
		free(elem);
		elem = next;
	}
	free(queue);
}


THREAD_RETURN jobqueue_executor(void *input) {
	thread_paramload(input);

	jobqueue_T queue = (jobqueue_T)input;
	int should_cancel = 0;
	while(!should_cancel) {
		// wait for change in jobs 
		thread_semaphoreclaim(queue->jobsemaphore);

		// if due to cancellation end loop 
		thread_mutexlock(queue->cancelmutex);
		if(queue->iscancelled)
			should_cancel = 1;
		thread_mutexrelease(queue->cancelmutex);
		if(should_cancel)
			break;	
		// else due to new job
		struct jobqueue_elem_T *nextjob;
		thread_mutexlock(queue->queuemutex);
			nextjob = queue->job_queue;
			queue->job_queue = nextjob->next;
		thread_mutexrelease(queue->queuemutex);

		queue->process_func(nextjob->data);
		free(nextjob);
	}

	
	thread_mutexlock(queue->stopmutex);
	queue->isstopped = 1;
	thread_mutexrelease(queue->stopmutex);
	thread_return(NULL);
}

void jobqueue_enqueue(jobqueue_T queue, void *data) {
	assert(queue);
	assert(!queue->isstopped);
	assert(!queue->iscancelled);

	struct jobqueue_elem_T *new_job;
	new_job = malloc(sizeof(*new_job));
	new_job->data = data;

	thread_mutexlock(queue->queuemutex);

	new_job->next = queue->job_queue;
	queue->job_queue = new_job;


	thread_semaphorerelease(queue->jobsemaphore);

	thread_mutexrelease(queue->queuemutex);

}

void jobqueue_cancel(jobqueue_T queue) {
	assert(queue);
	assert(!queue->isstopped);
	assert(!queue->iscancelled);

	thread_mutexlock(queue->cancelmutex);
	queue->iscancelled = 1;
	thread_semaphorerelease(queue->jobsemaphore);
	thread_mutexrelease(queue->cancelmutex);
}
