#include "bitstream.h"
#define BITSTREAM_INCREMENT 1 
#include <stdio.h>


int main() {


	bitstream_B stream = bitstream_new(1);

	bitstream_print(stream);

	printf("\n");


	bitstream_insert(stream, 0x55, 8);
	
	bitstream_print(stream);

	printf("\n");

	bitstream_big_endian_insert(stream, 0x55, 8);
	
	bitstream_print(stream);

	bitstream_trim(stream);

	printf("\n");

	bitstream_print(stream);

	printf("\n");

	bitstream_big_endian_insert(stream, 0x55, 8);
	
	bitstream_print(stream);

	printf("\n");

	bitstream_trim(stream);

	bitstream_print(stream);
}
