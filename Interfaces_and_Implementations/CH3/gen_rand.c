#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main() {

	int i;
	srand(time(NULL));
	printf("static unsigned long scatter[] = [");
	for(i = 0; i <= 256; i++) {
		int val = rand();
		printf("%d, ", val);
	}
	printf("\b\b];\n");


}
