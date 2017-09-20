#include "threadpool.h"
#include "thread.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define T threadpool_T
struct threadpool_emptyelem_T {
	unsigned int index;
	struct threadpool_emptyelem_T *next;
};
struct threadpool_taskelem_T {
	void (*func) (void *);
	void *data;
	struct threadpool_taskelem_T *next;
};

struct threadpool_worker_T {
	int index;
	unsigned char isstopped:1;	
	unsigned char iscancelled:1;	
	struct threadpool_taskelem_T *queue;
	struct threadpool_T *parent;

	thread_mutex_T stop_mutex;
	thread_mutex_T cancel_mutex;
	thread_mutex_T queue_mutex;
	
	thread_semaphore_T queue_semaphore;

	thread_T executer_thread;
};



struct threadpool_T {
	unsigned char iscancelled:1;
	unsigned char isstopped:1;
	unsigned char waitingforempty:1;
	unsigned char waitingforemptythread:1;
	struct threadpool_emptyelem_T *emptyqueue;
	struct threadpool_taskelem_T *taskqueue;
	struct threadpool_worker_T *workers;
	int thread_count;

	thread_mutex_T cancel_mutex;
	thread_mutex_T stop_mutex;
	thread_mutex_T emptyqueue_mutex;
	thread_mutex_T taskqueue_mutex;
	thread_mutex_T waitingforempty_mutex;
	thread_mutex_T waitingforemptythread_mutex;
	thread_semaphore_T taskqueue_sem;
	thread_semaphore_T emptyqueue_sem;
	thread_semaphore_T isempty_sem;
	thread_T executor_thread;
};



static void threadpool_workerinitialize(struct threadpool_worker_T *worker, int index, struct threadpool_T *parent);
static void threadpool_workercancel(struct threadpool_worker_T *worker);
static void threadpool_workerenqueue(struct threadpool_worker_T *worker, void (*func)(void *), void *data);
static THREAD_RETURN threadpool_workerfunction(void *arg);
static THREAD_RETURN threadpool_mainfunction(void *arg);
static void threadpool_workerenqueuefromelem(struct threadpool_worker_T *worker, struct threadpool_taskelem_T *new_job);

static THREAD_RETURN threadpool_mainfunction(void *arg) {
	thread_paramload(arg);
	struct threadpool_T *threadpool = arg;

	int should_cancel = 0;
	int next_assign = 0;
	while(!should_cancel) {
		thread_semaphoreclaim(threadpool->taskqueue_sem);
		// check if due to cancellation
		thread_mutexlock(threadpool->cancel_mutex);
			if(threadpool->iscancelled)
				should_cancel = 1;
		thread_mutexrelease(threadpool->cancel_mutex);
		if(should_cancel)
			break;

		// else due to new job
		//printf("Threadpool: Recieved a functions!\n");
		int queue_empty = 0;
		int waitingforempty = 0;
		struct threadpool_taskelem_T *next_job;

//		int i = 0;
		thread_mutexlock(threadpool->taskqueue_mutex);
//			struct threadpool_taskelem_T *t;
//			t = threadpool->taskqueue;
//			while(t != NULL) {
//				i++;
//				t = t->next;
//			}
//			printf("%d Tasks remaining before enqueing ", i);
			
			next_job = threadpool->taskqueue;
			threadpool->taskqueue = next_job->next;
			if(threadpool->taskqueue == NULL)
				queue_empty = 1;
		thread_mutexrelease(threadpool->taskqueue_mutex);

		thread_mutexlock(threadpool->waitingforempty_mutex);
			if(threadpool->waitingforempty)
				waitingforempty = 1;

			if(queue_empty && waitingforempty) {
				thread_semaphorerelease(threadpool->isempty_sem);
//				printf("Threadpool: EMPTY SIGNAL RELEASED\n");
			}  

		thread_mutexrelease(threadpool->waitingforempty_mutex);

			int assign_to = next_assign;
			// check if any empty
			thread_mutexlock(threadpool->emptyqueue_mutex);
				if(threadpool->emptyqueue != NULL) {
//					printf("ThreadPool: Retrieving queue\n");
					struct threadpool_emptyelem_T *empty;
					empty = threadpool->emptyqueue;
					assign_to = threadpool->emptyqueue->index;
					threadpool->emptyqueue = empty->next;
					free(empty);
				}
			thread_mutexrelease(threadpool->emptyqueue_mutex);
			// assign to the thread
//			printf(" to thread %d\n", assign_to);
			threadpool_workerenqueuefromelem(&threadpool->workers[assign_to], next_job);


			next_assign = (next_assign + 1) % threadpool->thread_count;

	}

	printf("Threadpool cancelled\n");
	// cancel all the workers
	int i;
	for(i = 0; i < threadpool->thread_count; ++i) {
		threadpool_workercancel(&threadpool->workers[i]);
		thread_join(threadpool->workers[i].executer_thread);
	}

	thread_mutexlock(threadpool->stop_mutex);
		threadpool->isstopped = 1;
	thread_mutexrelease(threadpool->stop_mutex);

	thread_return(NULL);
}
static THREAD_RETURN threadpool_workerfunction(void *arg) {
	thread_paramload(arg);
	struct threadpool_worker_T *worker = arg;

	int should_cancel = 0;
	while(!should_cancel) {
		// wait for task
//		printf("Thread[%d] sleeping\n", worker->index);
		thread_semaphoreclaim(worker->queue_semaphore);
//		printf("Thread[%d] awoke\n", worker->index);
		
		// check if due to cancellation
		thread_mutexlock(worker->cancel_mutex);
			if(worker->iscancelled)
				should_cancel = 1;
		thread_mutexrelease(worker->cancel_mutex);
		if(should_cancel) break;

//		printf("Thread[%d]: recieved a job!\n", worker->index);
		// else due to new job
		struct threadpool_taskelem_T *next_job;
		thread_mutexlock(worker->queue_mutex);
			next_job = worker->queue;
			worker->queue = next_job->next;

		// if empty signalempty
		if(next_job->next == NULL) {
			struct threadpool_emptyelem_T *emptyclaim;
			emptyclaim = malloc(sizeof(*emptyclaim));
			assert(emptyclaim);
			emptyclaim->index = worker->index;

			int should_signal = 0;
			thread_mutexlock(worker->parent->waitingforemptythread_mutex);
				if(worker->parent->waitingforemptythread)
					should_signal = 1;
			thread_mutexrelease(worker->parent->waitingforemptythread_mutex);
	
	
			thread_mutexlock(worker->parent->emptyqueue_mutex);
				emptyclaim->next = worker->parent->emptyqueue;
				worker->parent->emptyqueue = emptyclaim;
				if(should_signal) {
					thread_semaphorerelease(worker->parent->emptyqueue_sem);
//					printf("Thread [%d]: Signalling thread with new empty(%p)\n", worker->index, emptyclaim);
				}
			thread_mutexrelease(worker->parent->emptyqueue_mutex);
		}

		thread_mutexrelease(worker->queue_mutex);


		// run job
		next_job->func(next_job->data);
		free(next_job);
	}

//	printf("Thread[%d] cancelled(%p)\n", worker->index,worker);
	thread_mutexlock(worker->stop_mutex);
		worker->isstopped = 1;
	thread_mutexrelease(worker->stop_mutex);

	thread_return(NULL);
}
void threadpool_enqueue(T threadpool, void (*func) (void *), void * data) {
//	static int i = 1;
	assert(threadpool);
	assert(func);
	assert(data);

	struct threadpool_taskelem_T *new_job;
	new_job = malloc(sizeof(*new_job));
	assert(new_job);
	new_job->func = func;
	new_job->data = data;
//	printf("Enqueuing task %d\n", i++);

	thread_mutexlock(threadpool->taskqueue_mutex);
		new_job->next = threadpool->taskqueue;
		threadpool->taskqueue = new_job;
		thread_semaphorerelease(threadpool->taskqueue_sem);
	thread_mutexrelease(threadpool->taskqueue_mutex);
}
static void threadpool_workerenqueuefromfunc(struct threadpool_worker_T *worker, void (*func)(void *), void *data) {
	assert(worker);
	assert(func);
	assert(data);
	struct threadpool_taskelem_T *new_job;
	new_job = malloc(sizeof(*new_job));
	assert(new_job);
	new_job->func = func;
	new_job->data = data;

	thread_mutexlock(worker->queue_mutex);
		new_job->next = worker->queue;
		worker->queue = new_job;
		thread_semaphorerelease(worker->queue_semaphore);
	thread_mutexrelease(worker->queue_mutex);
}
static void threadpool_workerenqueuefromelem(struct threadpool_worker_T *worker, struct threadpool_taskelem_T *new_job) {
	assert(worker);
	assert(new_job);
	thread_mutexlock(worker->queue_mutex);
		new_job->next = worker->queue;
		worker->queue = new_job;
		thread_semaphorerelease(worker->queue_semaphore);
	thread_mutexrelease(worker->queue_mutex);
}

static void threadpool_workercancel(struct threadpool_worker_T *worker) {
	
//	printf("Cancelling thread[%d]\n", worker->index);
	thread_mutexlock(worker->cancel_mutex);
		worker->iscancelled = 1;
		thread_semaphorerelease(worker->queue_semaphore);
	thread_mutexrelease(worker->cancel_mutex);
}

static void threadpool_workerinitialize(struct threadpool_worker_T *worker, int index, struct threadpool_T *parent) {
	assert(worker);
	assert(parent);
	worker->index = index;
	worker->isstopped = 0;
	worker->iscancelled = 0;
	worker->queue = NULL;
	worker->parent = parent;
	worker->stop_mutex = thread_mutexnew();
	worker->cancel_mutex = thread_mutexnew();
	worker->queue_mutex = thread_mutexnew();
	worker->queue_semaphore = thread_semaphorenew(0);
	worker->executer_thread = thread_new(threadpool_workerfunction, worker);
	return;

}
static void threadpool_workerdelete(struct threadpool_worker_T *worker) {
	assert(worker);
	struct threadpool_taskelem_T *tasks;
	int should_cancel = 0;

	thread_mutexlock(worker->stop_mutex);
		if(!worker->isstopped)
			should_cancel = 1;
	thread_mutexrelease(worker->stop_mutex);

	if(should_cancel) 
		threadpool_workercancel(worker);


	tasks = worker->queue;
	while(tasks != NULL) {
		struct threadpool_taskelem_T *task = tasks->next;
		free(tasks);
		tasks = task;
	}
	thread_mutexdelete(worker->stop_mutex);
	thread_mutexdelete(worker->cancel_mutex);
	thread_mutexdelete(worker->queue_mutex);
	thread_semaphoredelete(worker->queue_semaphore);
	thread_delete(worker->executer_thread);
}

void threadpool_cancel(T threadpool) {
	assert(threadpool);
	thread_mutexlock(threadpool->cancel_mutex);
		threadpool->iscancelled = 1;
		thread_semaphorerelease(threadpool->taskqueue_sem);
	thread_mutexrelease(threadpool->cancel_mutex);
}

T threadpool_new(unsigned int thread_count) {
	T threadpool;
	threadpool = malloc(sizeof(*threadpool));

	assert(threadpool);

	threadpool->isstopped = 0;
	threadpool->iscancelled = 0;
	threadpool->waitingforempty = 0;
	threadpool->waitingforemptythread = 0;
	threadpool->emptyqueue = NULL;
	threadpool->taskqueue = NULL;
	threadpool->workers = malloc(sizeof(*threadpool->workers)*thread_count);

	assert(threadpool->workers);
	
	int i;
	for(i = 0; i < (int) thread_count; ++i) {
		threadpool_workerinitialize(&threadpool->workers[i], i, threadpool);
	}
	threadpool->thread_count = (int)thread_count;
	threadpool->cancel_mutex = thread_mutexnew();
	threadpool->stop_mutex = thread_mutexnew();
	threadpool->waitingforempty_mutex = thread_mutexnew();
	threadpool->waitingforemptythread_mutex = thread_mutexnew();
	threadpool->emptyqueue_mutex = thread_mutexnew();
	threadpool->taskqueue_mutex = thread_mutexnew();

	threadpool->isempty_sem = thread_semaphorenew(0);
	threadpool->emptyqueue_sem = thread_semaphorenew(0);
	threadpool->taskqueue_sem = thread_semaphorenew(0);
	threadpool->executor_thread = thread_new(threadpool_mainfunction, threadpool);
	

	return threadpool;
}

void threadpool_delete(T threadpool) {
	assert(threadpool);
	int should_cancel = 0;
	thread_mutexlock(threadpool->stop_mutex);
		if(threadpool->isstopped) should_cancel = 1;
	thread_mutexrelease(threadpool->stop_mutex);

	if(should_cancel)
		threadpool_cancel(threadpool);
	struct threadpool_taskelem_T *tasks;
	struct threadpool_emptyelem_T *emptys;


	 int i;
	 for(i = 0; i <threadpool->thread_count; ++i) {
		threadpool_workerdelete(&threadpool->workers[i]);
	 }

	tasks = threadpool->taskqueue;
	while(tasks != NULL) {
		struct threadpool_taskelem_T *task = tasks->next;
		free(tasks);
		tasks = task;
	}
	 emptys = threadpool->emptyqueue;
	 while(emptys != NULL) {
		 struct threadpool_emptyelem_T *empty = emptys->next;
		 free(emptys);
		 emptys = empty;
	 }
	
	 free(threadpool->workers);

	thread_mutexdelete(threadpool->cancel_mutex);
	thread_mutexdelete(threadpool->stop_mutex);
	thread_mutexdelete(threadpool->emptyqueue_mutex);
	thread_mutexdelete(threadpool->taskqueue_mutex);
	thread_mutexdelete(threadpool->waitingforempty_mutex);
	thread_mutexdelete(threadpool->waitingforemptythread_mutex);
	thread_semaphoredelete(threadpool->taskqueue_sem);
	thread_semaphoredelete(threadpool->emptyqueue_sem);
	thread_semaphoredelete(threadpool->isempty_sem);
	thread_delete(threadpool->executor_thread);

	free(threadpool);
}
void threadpool_join(T threadpool) {
	assert(threadpool);
//	printf("Joining threadpool\n");

	int task_queue_empty = 0;
	thread_mutexlock(threadpool->taskqueue_mutex);
		if(threadpool->taskqueue != NULL)
			task_queue_empty = 1;
	thread_mutexrelease(threadpool->taskqueue_mutex);
//	printf("Joining threadpool - checked whether task queue empty\n");

	if(task_queue_empty) {
		thread_mutexlock(threadpool->waitingforempty_mutex);
			threadpool->waitingforempty = 1;
		thread_mutexrelease(threadpool->waitingforempty_mutex);
//		printf("Joining threadpool - waiting for queue empty, set waiting for empty\n");
		
		while(task_queue_empty) {
			// wait for isempty signal
			thread_semaphoreclaim(threadpool->isempty_sem);
//			printf("Joining threadpool - recieved is empty sem\n");

			thread_mutexlock(threadpool->taskqueue_mutex);
				if(threadpool->taskqueue == NULL) {
					task_queue_empty = 0;

				// move down outside of while
					thread_mutexlock(threadpool->waitingforempty_mutex);
						threadpool->waitingforempty = 0;
					thread_mutexrelease(threadpool->waitingforempty_mutex);

					thread_mutexlock(threadpool->waitingforemptythread_mutex);
						threadpool->waitingforemptythread = 1;
					thread_mutexrelease(threadpool->waitingforemptythread_mutex);
	
				}
			thread_mutexrelease(threadpool->taskqueue_mutex);

			if(!task_queue_empty) break;
		}



	}

//	printf("Joining threadpool - taskqueue empty\n");

	// at this point the input queue is empty
	int empty_threads[threadpool->thread_count];
	int i;
	for(i = 0; i <threadpool->thread_count; ++i){
		struct threadpool_worker_T *worker = &threadpool->workers[i];
		thread_mutexlock(worker->queue_mutex);
			if(worker->queue == NULL)
				empty_threads[i] = 1;
			else
				empty_threads[i] = 0;
		thread_mutexrelease(worker->queue_mutex);
//		printf("Thread[%d]: %s\n", i, (empty_threads[i] ? "empty" : "not empty"));
	}
	while(1) {
		int should_exit = 1;
		for(i = 0; i < threadpool->thread_count; ++i)
			if(!empty_threads[i]) should_exit = 0;
		if(should_exit) break;
		
		// wait for a signal 
		thread_semaphoreclaim(threadpool->emptyqueue_sem);
//		printf("Joining threadpool - Recieved an empty queue signal\n");

			int index;
		thread_mutexlock(threadpool->emptyqueue_mutex);
			struct threadpool_emptyelem_T *next = threadpool->emptyqueue;
//			printf("Joining Threadpool empty queue is %p\n", next);
			if(next != NULL) {
				threadpool->emptyqueue = threadpool->emptyqueue->next;
				index = next->index;
		//	printf("Joining threadpool - freeing next\n");
				free(next);
				empty_threads[index] = 1;
			}
		thread_mutexrelease(threadpool->emptyqueue_mutex);
	}
	// at this point all threads are cancelled

	threadpool_cancel(threadpool);
	// finally join
	thread_join(threadpool->executor_thread);
}


#undef T
