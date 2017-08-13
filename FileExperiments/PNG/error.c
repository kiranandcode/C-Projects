#include <stdio.h>
#include <stdlib.h>

void error(char *error){
	fprintf(stderr, "PNG Encoder Error: %s", error);
	exit(1);
}
