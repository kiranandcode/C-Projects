#include "thread.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

#define NUM_TOTAL_BUFFERS 5
#define DATA_LENGTH 20
struct data {
	char *buffers;
	thread_semaphore_T emptybuffers;
	thread_semaphore_T fullbuffers;
};

static void ProcessData(char data) {
	int i = (((data * 13) % 23) * 15 )% 7;
	thread_sleep(i > 5 ?  i : 5);
}

static char PrepareData(void) {
	static char *str = "Kiranisawesomeandyouknowthathisisthecase";
	static int pos = 0;
	return str[pos++];
}

static THREAD_RETURN writer_workerfunc(void *arg) {
	thread_paramload(arg);

		struct data *data = arg;
		thread_semaphore_T emptybuffers,fullbuffers;
		char *buffers = data->buffers;
		emptybuffers = data->emptybuffers;
		fullbuffers = data->fullbuffers;
		

		int i, writePt = 0;
		char c;
		for(i = 0; i < DATA_LENGTH; i++) {
			c = PrepareData();
			thread_semaphoreclaim(emptybuffers);
			buffers[writePt] = c;
			printf("Writer: buffer[%d] = %c\n", writePt, c);
			writePt = (writePt + 1) % 
				NUM_TOTAL_BUFFERS;
			thread_semaphorerelease(fullbuffers);
		}


	thread_return(NULL);
}
static THREAD_RETURN reader_workerfunc(void *arg) {
	thread_paramload(arg);
		struct data *data = arg;
		thread_semaphore_T emptybuffers,fullbuffers;
		char *buffers = data->buffers;
		emptybuffers = data->emptybuffers;
		fullbuffers = data->fullbuffers;

		int i, readPt = 0;
		char c;

		for(i = 0; i < DATA_LENGTH; i++) {
			thread_semaphoreclaim(fullbuffers);
			c = buffers[readPt];
			printf("\t\tReader: buffer[%d] = %c\n",readPt, c);
			readPt = (readPt + 1) % NUM_TOTAL_BUFFERS;
			ProcessData(c);
			thread_semaphorerelease(emptybuffers);
		}
	thread_return(NULL);
}

int main() {
	thread_semaphore_T emptybuffers,fullbuffers;
	emptybuffers = thread_semaphorenew(NUM_TOTAL_BUFFERS);
	fullbuffers  = thread_semaphorenew(0);

	char buffers[NUM_TOTAL_BUFFERS];
	struct data args = {
		buffers, emptybuffers, fullbuffers
	};

	thread_T writer = 
		thread_new(writer_workerfunc, &args);
       thread_T reader = 
       		thread_new(reader_workerfunc, &args);	       

       thread_join(writer);
       thread_join(reader);
       thread_semaphoredelete(emptybuffers);
       thread_semaphoredelete(fullbuffers);
       printf("All done!\n");
}
