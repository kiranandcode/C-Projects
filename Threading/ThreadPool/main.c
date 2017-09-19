#include "threadpool.h"
#include "thread.h"

#include <stdio.h>
struct delay {
	int delay;
	char *string;
};

int shouldnotcancel = 1;

struct delay datas[] = {
	{   10, "Task[1]:   10s delay" },
	{    9, "Task[2]:   9s  delay" },
	{   12, "Task[3]:   12s delay" },
	{   10, "Task[4]:   10s delay" },
	{    8, "Task[5]:   8s  delay" },
	{    0, "Task[6]:   0s  delay" }
};

void print_string_after_delay(void *data) {
	struct delay *item = data;
	printf("Waiting(%d) for a thing\n", item->delay);
	if(item->delay){
		thread_sleep(item->delay);
		printf("%s\n", item->string);
	} else 
		shouldnotcancel = 0;
}

int main() {
	threadpool_T threadpool = threadpool_new(3);
	
	int i;
	for(i = 0; i < 6; i++) {
		threadpool_enqueue(threadpool, print_string_after_delay,  &datas[i]);
	}

	while(shouldnotcancel);
	threadpool_cancel(threadpool);
	threadpool_join(threadpool);

	threadpool_delete(threadpool);
}
