#include <stdio.h>
#include "../includes/countdown.h"

void printCountdown() {
	int i;
	for(i = 0; i<10; i++) {
		printf("Counting %d down\n", (10-i));
	}
}
