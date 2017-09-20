#include "thread.h"
#include "list.h"

#include <stdlib.h>
#include <stdio.h>

#define NUM_WATER 10
struct data {
	thread_semaphore_T oxygenReady;
	thread_semaphore_T hydrogenReady;
	int thread_count;
};


static THREAD_RETURN oxygen_workerfunc(void *args) {
	thread_paramload(args);
		struct data *data = args;
		thread_semaphore_T oxygenReady  = data->oxygenReady;
		thread_semaphore_T hydrogenReady = data->hydrogenReady;
		int thread_count = data->thread_count;

		printf("Oxygen worker waiting on hydrogen\n");
		thread_semaphoreclaim(hydrogenReady);
		printf("Oxygen worker waiting on hydrogen\n");
		thread_semaphoreclaim(hydrogenReady);
		printf("Oxygen worker releasing oxygen\n");
		thread_semaphorerelease(oxygenReady);
		printf("Oxygen worker releasing oxygen\n");
		thread_semaphorerelease(oxygenReady);
	
		printf("Water made\n");
	thread_return(NULL);
}
static THREAD_RETURN hydrogen_workerfunc(void *args) {
	thread_paramload(args);
		struct data *data = args;
		thread_semaphore_T oxygenReady  = data->oxygenReady;
		thread_semaphore_T hydrogenReady = data->hydrogenReady;
		int thread_count = data->thread_count;

		printf("Hydrogen worker releasing hydrogen\n");
		thread_semaphorerelease(hydrogenReady);
		printf("Hydrogen worker waiting on oxygen\n");
		thread_semaphoreclaim(oxygenReady);

	thread_return(NULL);
}

int main() {
	int i;
	thread_semaphore_T oxygenReady, hydrogenReady;
	list_L oxygenthreads, hydrogenthreads;
	struct data args[NUM_WATER];

	oxygenReady = thread_semaphorenew(0);
	hydrogenReady = thread_semaphorenew(0);


	oxygenthreads = list_new();
	hydrogenthreads = list_new();

	for(i = 0; i < NUM_WATER; i++) {
		args[i].hydrogenReady = hydrogenReady;
		args[i].oxygenReady = oxygenReady;
		args[i].thread_count = i;
	}
	for(i = 0; i < NUM_WATER; i++) 
		list_push(oxygenthreads, thread_new(oxygen_workerfunc,&args[i]));

	for(i = 0; i < 2 * NUM_WATER; i++) 
		list_push(hydrogenthreads, thread_new(hydrogen_workerfunc,&args[i % NUM_WATER]));
	
	struct L_iterator iter = list_iterator(oxygenthreads);
	while(list_iteratorhasnext(&iter)) {
		thread_T thread = list_iteratornext(&iter);
		thread_join(thread);
		thread_delete(thread);
	}


	iter = list_iterator(hydrogenthreads);
	while(list_iteratorhasnext(&iter)) {
		thread_T thread = list_iteratornext(&iter);
		thread_join(thread);
		thread_delete(thread);
	}
	printf("All done\n");

	thread_semaphoredelete(hydrogenReady);
	thread_semaphoredelete(oxygenReady);
}
