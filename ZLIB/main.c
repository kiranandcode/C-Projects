#define DEBUG
#include "bitstream.h"
#include "deflate.h"
#define BITSTREAM_INCREMENT 1 
#include <stdio.h>


int main() {

//	printf("Inserting 12\n");

	bitstream_B stream = bitstream_new(1);
	deflate_insert(stream, 12);


	bitstream_print(stream);
	printf("\n");

//	printf("Inserting 240\n");
	bitstream_B another = bitstream_new(1);
	deflate_insert(another, 61);
	
	int i;
	for(i = 0; i< 10; i++) printf("-");
	bitstream_print(another);
	printf("\n");

	
//	printf("Inserting 240\n");
	deflate_insert(stream, 61);
	bitstream_print(stream);
	printf("\n");
}
