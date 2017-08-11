#include <stdlib.h>
#include <stdio.h>


void error(int linenum, int columnum, char *error) {
	fprintf(stderr, "ERROR [Line %d, Col %d]: %s", 
			linenum,
			columnum,
			error);
	exit(1);

}
