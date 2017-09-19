#include "threadpool.h"
#include "thread.h"

#include <stdio.h>
struct delay {
	int delay;
	char *string;
};


struct delay datas[] = {
	{   0, "\t\t\t\tTask[1]:    0s delay" },
	{    9, "\t\t\t\tTask[2]:   9s  delay" },
	{   12, "\t\t\t\tTask[3]:   12s delay" },
	{   0, "\t\t\t\tTask[4]:    0s delay" },
	{    8, "\t\t\t\tTask[5]:   8s  delay" },
	{    0, "\t\t\t\tTask[6]:   0s  delay" }
};

void print_string_after_delay(void *data) {
	struct delay *item = data;
	if(item->delay){
		thread_sleep(item->delay);
	} 
	printf("%s\n", item->string);
}

int main() {
	threadpool_T threadpool = threadpool_new(6);
	
	int i;
	for(i = 0; i < 6; i++) {
		threadpool_enqueue(threadpool, print_string_after_delay,  &datas[i]);
	}

	
	threadpool_join(threadpool);

//	threadpool_delete(threadpool);
}
