#include "thread.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

#define NUM_DINERS 5
#define EAT_TIMES 3

#define LEFT(philNum) (philNum)
#define RIGHT(philNum) (((philNum)+1)%NUM_DINERS)
struct data {
	char *name;
	thread_semaphore_T numEating;
	thread_semaphore_T forkLeft;
	thread_semaphore_T forkRight;
};

static THREAD_RETURN philosopher_genericworker(void *args) {
	thread_paramload(args);
	struct data *data = args;
	thread_semaphore_T numEating = data->numEating;
	thread_semaphore_T forkLeft = data->forkLeft;
	thread_semaphore_T forkRight = data->forkRight;
	int i;
	int random_gen = data->name[12] + 1;
	for(i = 0; i < EAT_TIMES; i++) {
		printf("%s thinking\n", data->name);
		thread_semaphoreclaim(numEating);
		thread_semaphoreclaim(forkLeft);
		thread_semaphoreclaim(forkRight);

		printf("%s eating!\n", data->name);
		int tts = ((((random_gen * 13)%23)*43)%7);
		thread_sleep(tts > 3 ? tts : 3);
		random_gen = tts;

		thread_semaphorerelease(forkRight);
		thread_semaphorerelease(forkLeft);
		thread_semaphorerelease(numEating);
	}
	

	thread_return(NULL);
}

static THREAD_RETURN philosopher_rightleftworker(void *args) {
	thread_paramload(args);
	struct data *data = args;
	thread_semaphore_T forkLeft = data->forkLeft;
	thread_semaphore_T forkRight = data->forkRight;
	int i;
	int random_gen = data->name[12] + 1;
	for(i = 0; i < EAT_TIMES; i++) {
		printf("%s thinking\n", data->name);
		thread_semaphoreclaim(forkRight);
		thread_semaphoreclaim(forkLeft);

		printf("%s eating!\n", data->name);
		int tts = ((((random_gen * 13)%23)*43)%7);
		thread_sleep(tts > 3 ? tts : 3);
		random_gen = tts;

		thread_semaphorerelease(forkLeft);
		thread_semaphorerelease(forkRight);
	}
	

	thread_return(NULL);
}
static THREAD_RETURN philosopher_leftrightworker(void *args) {
	thread_paramload(args);
	struct data *data = args;
	thread_semaphore_T forkLeft = data->forkLeft;
	thread_semaphore_T forkRight = data->forkRight;
	int i;
	int random_gen = data->name[12] + 1;
	for(i = 0; i < EAT_TIMES; i++) {
		printf("%s thinking\n", data->name);
		thread_semaphoreclaim(forkLeft);
		thread_semaphoreclaim(forkRight);

		printf("%s eating!\n", data->name);
		int tts = ((((random_gen * 13)%23)*43)%7);
		thread_sleep(tts > 3 ? tts : 3);
		random_gen = tts;

		thread_semaphorerelease(forkRight);
		thread_semaphorerelease(forkLeft);
	}
	

	thread_return(NULL);
}


int main (){ 
	int i;
	char names[NUM_DINERS][32];
	struct data args[NUM_DINERS];
	thread_semaphore_T fork[NUM_DINERS];
	thread_semaphore_T numEating;
	list_L philosophers = list_new();

	for(i = 0; i <  NUM_DINERS; i++) {
		fork[i] = thread_semaphorenew(1);
	}
	numEating = thread_semaphorenew(NUM_DINERS - 1);

	for(i = 0; i < NUM_DINERS; ++i) {
		sprintf(names[i], "Philosopher %d", i);
		args[i].name = names[i];
		args[i].numEating = numEating;
		args[i].forkLeft = fork[LEFT(i)];
		args[i].forkRight = fork[RIGHT(i)];
		// Basic solution, limit throughput to avoid deadlock
		//list_push(philosophers, thread_new(philosopher_genericworker, &args[i]));
		
		// Custom solution, alternate preference philosophers next to each other
		if(i % 2) {
			list_push(philosophers, thread_new(philosopher_leftrightworker, &args[i]));
		} else {
			list_push(philosophers, thread_new(philosopher_rightleftworker, &args[i]));
		}
	}

	struct L_iterator iter = list_iterator(philosophers);

	while(list_iteratorhasnext(&iter)) {
		thread_T thread = list_iteratornext(&iter);
		thread_join(thread);
		thread_delete(thread);
	}

	printf("All done\n");
	thread_semaphoredelete(numEating);
	for(i = 0; i < NUM_DINERS; ++i) {
		thread_semaphoredelete(fork[i]);
	}
}

