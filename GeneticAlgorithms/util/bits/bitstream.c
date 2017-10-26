#include "bitstream.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
// #define DEBUG

#define B bitstream_B
#define NEW(o) ((o) = malloc(sizeof(*(o))))

static unsigned char individual_bit_masks[] = {
	/* 0 */ //0x00,
	/* 1 */ 0x01,
	/* 2 */ 0x02,
	/* 3 */ 0x04,
	/* 4 */ 0x08,
	/* 5 */ 0x10,
	/* 6 */ 0x20,
	/* 7 */ 0x40,
	/* 8 */ 0x80
};

static unsigned char bit_masks[] = {
	/* 0000 0000 */ 0x00,
	/* 0000 0001 */ 0x01,
	/* 0000 0011 */ 0x03,
	/* 0000 0111 */ 0x07,
	/* 0000 1111 */ 0x0F,
	/* 0001 1111 */ 0x1F,
	/* 0011 1111 */ 0x3F,
	/* 0111 1111 */ 0x7F, /* 1111 1111 */ 0xFF
};

struct B {
	char *stream;
	unsigned int position; /* byte position in the bitstream */
	unsigned int offset;   /* offset in bits in current byte */
	unsigned int length;   /* byte length in bitstream */
};


B bitstream_new(unsigned int length) {
	assert(length > 0);
	B bitstream;
	NEW(bitstream);

	bitstream->length = length;
	bitstream->stream = calloc(sizeof(char), bitstream->length);
	bitstream->position = 0;
	bitstream->offset = 0;
	return bitstream;
}

void bitstream_delete(B bitstream) {
	free(bitstream->stream);
	free(bitstream);
}


B bitstream_copy(B bitstream) {
	B copy;
	NEW(copy);
	copy->length = bitstream->length;
	copy->position = bitstream->position;
	copy->offset  = bitstream->offset;
	copy->stream = calloc(sizeof(char), bitstream->length);
	int i;

	for(i = 0; i <(int) copy->position+1; ++i) {
		copy->stream[i] = bitstream->stream[i];
	}

	return copy;
}


B bitstream_concat(B bitstream_a, B bitstream_b) {
	B result = bitstream_copy(bitstream_a);
	
	int i;
	for(i = 0; i < (int) bitstream_b->position+1; ++i) {
		if(i != (int)bitstream_b->position) {
			bitstream_insert(result, (unsigned char)bitstream_b->stream[i], 8);
		} else {
			if(bitstream_b->offset)
			bitstream_insert(result, bitstream_b->stream[i], bitstream_b->offset);
		}
	}
	return result;
}
void bitstream_insert(B stream, unsigned char sequence, unsigned int bitlength) {
	assert(bitlength <= 8 && bitlength >= 1);
	assert(stream->offset < 8 && stream->offset >= 0);
#ifdef FIXED_SIZE
	assert(stream->position < stream->length);
#endif
#ifdef DEBUG
	printf("Before inserting %#x, offset %u, position %u, length %u, bitlength %u\n",
			sequence, stream->offset, stream->position, stream->length, bitlength);
#endif



	unsigned char first_byte = ((sequence & bit_masks[bitlength]) << stream->offset) & ~bit_masks[stream->offset];
	unsigned char second_byte = 0;
//	printf("first_byte = %#x\n", first_byte);
	stream->stream[stream->position] |= first_byte;

	if(stream->offset + bitlength >= 8) {

		second_byte =  (sequence >> (8 - (int)stream->offset)) & bit_masks[(stream->offset + bitlength - 8)];

//	printf("second_byte = %#x\n", second_byte);

		stream->position++;
#ifdef FIXED_SIZE
		if(stream->position < stream->length)
			stream->stream[stream->position] |= second_byte;
#else
		if(stream->position >= stream->length)
		{
			stream->length = stream->length + STREAM_INCREMENT_SIZE;
			stream->stream = realloc(stream->stream, sizeof(char) * (stream->length));
			int i;
			for(i = stream->position; i < (int)stream->length; ++i) {
				stream->stream[i] = 0;
			}
		}
		
		stream->stream[stream->position] |= second_byte;
#endif
	}
	stream->offset = (stream->offset + bitlength) % 8;

#ifdef DEBUG
	printf("After inserting %#x, offset %u, position %u, length %u\n\n",
			sequence, stream->offset, stream->position, stream->length);
#endif
}


static unsigned char to_big_endian(unsigned char sequence) {

	unsigned char converted = 0;
	int i;
	for(i = 0; i < 8; ++i) {
		if((sequence >> i) & 1) 
			converted |= individual_bit_masks[7-i];
	}

	return converted;
}

void bitstream_big_endian_insert(B stream, unsigned char sequence, unsigned int bitlength) {
	assert(bitlength <= 8 && bitlength >= 1);
	assert(stream->offset < 8 && stream->offset >= 0);
#ifdef FIXED_SIZE
	assert(stream->position < stream->length);
#endif
#ifdef DEBUG
	printf("Before inserting %#x, offset %u, position %u, length %u, bitlength %u\n",
			sequence, stream->offset, stream->position, stream->length, bitlength);
#endif


	sequence = to_big_endian(sequence);
	sequence >>= 8 -(int) bitlength;
	
	unsigned char first_byte = ((sequence & bit_masks[bitlength]) << stream->offset) & ~bit_masks[stream->offset];
	unsigned char second_byte = 0;
	stream->stream[stream->position] |= first_byte;

	if(stream->offset + bitlength >= 8) {

		second_byte =  (sequence >> (8 - (int)stream->offset)) & bit_masks[(stream->offset + bitlength - 8)];
		//second_byte =  (sequence >> (stream->offset + bitlength - 8)) & bit_masks[(stream->offset + bitlength - 8)];


		stream->position++;
#ifdef FIXED_SIZE
		if(stream->position < stream->length)
			stream->stream[stream->position] |= second_byte;
#else
		if(stream->position >= stream->length)
		{
			stream->stream = realloc(stream->stream, sizeof(char) * (stream->length + STREAM_INCREMENT_SIZE));
			stream->length += STREAM_INCREMENT_SIZE;
			int i;
			for(i = stream->position; i < (int)stream->length; ++i) {
				stream->stream[i] = 0;
			}
		}
		stream->stream[stream->position] |= second_byte;
#endif
	}
	stream->offset = (stream->offset + bitlength) % 8;

#ifdef DEBUG
	printf("After inserting %#x, offset %u, position %u, length %u, bitlength %u\n\n",
			sequence, stream->offset, stream->position, stream->length, bitlength);
#endif
}

void bitstream_trim(B bitstream) {
	if(bitstream->length > bitstream->position) {
		bitstream->length = bitstream->position + 1;
		bitstream->stream = realloc(bitstream->stream, sizeof(char) * (bitstream->length));
	}


}

void bitstream_get_stream(B bitstream, unsigned char *stream, unsigned int length) {

	assert(bitstream);
	assert(stream);
	int i;
	int max_length = length > bitstream->length ? bitstream->length : length;

	for(i = 0; i < max_length; i++) {
		stream[i] = bitstream->stream[i];
	}
}

void print_char(unsigned char val) {
	int j;
		for(j = 0; j < 8; ++j) {
			if(1 & (val >> j))
				printf("1");
			else printf("0");
		}
		printf("\n");
}


void bitstream_print(B stream) {
	int i,j;

	for(i = 0; i <(int)stream->length; ++i) {

		unsigned char current_bytes = stream->stream[i];
		for(j = 0; j < 8; ++j) {
			if(1 & (current_bytes >> j))
				printf("1");
			else printf("0");
		}
//		if(i != (int)stream->length -1)
//			printf(",");
	}
}
