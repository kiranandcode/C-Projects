#include "jobqueue.h"
#include <stdio.h>
#include <stdlib.h>

struct data {
	int count;
	char c;
};

void execute_data(void *data) {
	struct data *item = data;
	int i;
	int count = item->count;
	char c = item->c;
	free(item);
	for(i = 0; i < count; i++) {
		printf("%c(%d)\n", c, i);
	}
}

int main() {

	char c;
	jobqueue_T queue = jobqueue_new(execute_data);
	do {
		int count;
		char ch = 'n';
		struct data *data;
		printf("Enter a count:");
		scanf(" %d", &count);
		printf("Enter a character:");
		scanf(" %c", &ch);
		data = malloc(sizeof(*data));
		data->c = ch;
		data->count = count;
		jobqueue_enqueue(queue, data);

		printf("Would you like to continue?(Y/n)\n");
		scanf(" %c", &c);
	} while(c != 'n');
	jobqueue_delete(queue);
}
