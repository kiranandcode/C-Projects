#include "input.h"

#include <stdio.h>


int main() {
	FILE *fp;

	fp = fopen("test.txt", "r");

	if(fp != NULL ){
		filebuffer_F buffer = filebuffer_new(fp);

		do {
			printf("[%d]: ", filebuffer_lineno(buffer));
			char *cp = filebuffer_cp(buffer);
			while(*cp != '\n' && cp < filebuffer_limit(buffer)) {
				printf("%c", *cp);
				cp++;
				filebuffer_incrementcp(buffer);
			}
			printf("\n");
		} while(filebuffer_nextline(buffer) != filebuffer_limit(buffer));
	}
}
