#define DEBUG
#include "bitstream.h"
#include "deflate.h"
#define BITSTREAM_INCREMENT 1 
#include <stdio.h>


int main() {

//	printf("Inserting 12\n");

	bitstream_B stream = bitstream_new(1);
        deflate_distance_insert(stream, 4);


	bitstream_print(stream);
	printf("\n");

//	printf("Inserting 240\n");
	bitstream_B another = bitstream_new(1);
	deflate_distance_insert(another, 1047);
	
	int i;
	for(i = 0; i< 5; i++) printf("-");
	bitstream_print(another);
	printf("\n");

	
//	printf("Inserting 240\n");
	deflate_distance_insert(stream, 1047);
	bitstream_print(stream);
	printf("\n");
}
