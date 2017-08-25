#define DEBUG
#include "bitstream.h"
#define BITSTREAM_INCREMENT 1 
#include <stdio.h>


int main() {


	bitstream_B stream = bitstream_new(1);

	bitstream_insert(stream, 0x55, 8);

	bitstream_big_endian_insert(stream, 0x55, 8);
	
	bitstream_big_endian_insert(stream, 0x55, 3);
	
	//bitstream_trim(stream);

	bitstream_print(stream);

	printf("\n");

        bitstream_B copy = bitstream_copy(stream);

	bitstream_print(copy);

	printf("\n");
	
	bitstream_B concat = bitstream_concat(stream,copy);

	bitstream_print(concat);

}
