#include "threadpool.h"
#include "thread.h"
#include <time.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
struct delay {
	int delay;
	int task_count;
};

int *execute_count;

struct timespec start,end;
double cpu_time_used;

/*struct delay datas[] = {
	{   0, 1, "\t\t\t\tTask[1]:    0s delay" },
	{    9,  "\t\t\t\tTask[2]:   9s  delay" },
	{   12, "\t\t\t\tTask[3]:   12s delay" },
	{   0, "\t\t\t\tTask[4]:    0s delay" },
	{    8, "\t\t\t\tTask[5]:   8s  delay" },
	{    0, "\t\t\t\tTask[6]:   0s  delay" }
};*/

void print_string_after_delay(void *data) {
	struct delay *item = data;
	if(item->delay){
		thread_sleep(item->delay);
/*		struct timespec begin,current;
		clock_gettime(CLOCK_MONOTONIC_RAW, &begin);
		double time_displacement = 0;
		do {
			clock_gettime(CLOCK_MONOTONIC_RAW, &current);

			time_displacement = (current.tv_nsec - begin.tv_nsec)/1000000000.0 + (current.tv_sec - begin.tv_sec);
		} while( time_displacement < item->delay);*/
	} 
	printf("\t\t\t\t\t\tTask[%d]: %ds delay\n", item->task_count, item->delay);
	execute_count[item->task_count] = 1;
}

#define data_count 5
int main() {
	struct delay *datas;
	datas =  malloc(sizeof(*datas) * data_count);
	execute_count = malloc(sizeof(*execute_count) * data_count);
	int i;
	int rndgen = 1;
	for(i = 0; i < data_count; i++) {
		execute_count[i] = 0;
		datas[i].delay = (((rndgen *11) % 23)*13)%15;
		rndgen = datas[i].delay;
		datas[i].task_count = i;
	}

	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	threadpool_T threadpool = threadpool_new(1);


	for(i = 0; i < data_count; i++) {
		threadpool_enqueue(threadpool, print_string_after_delay,  &datas[i]);
	}
	
	threadpool_join(threadpool);
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);

	printf("%lf s taken of ",
		(end.tv_nsec - start.tv_nsec)/1000000000.0 +
		(end.tv_sec - start.tv_sec));
	int sum = 0;
	for(i = 0; i < data_count; ++i) {
		sum += datas[i].delay;
	}
	printf("%d total task time\n", sum);
	int any_failed = 0;
	for(i = 0; i < data_count; ++i){
		if(execute_count[i] == 0) {
			any_failed = 1;
			printf("Task[%d]: Failed to Execute\n", i);
		}
	}
	if(!any_failed) {
		printf("All tasks executed successfully\n");
	}

//	threadpool_delete(threadpool);
}
