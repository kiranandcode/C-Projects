#include "bitstream.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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
	/* 0111 1111 */ 0x7F,
	/* 1111 1111 */ 0xFF
};

struct B {
	char *stream;
	unsigned int position; /* byte position in the bitstream */
	unsigned int offset;   /* offset in bits in current byte */
	unsigned int length;   /* byte length in bitstream */
};


B bitstream_new(unsigned int length) {
	B bitstream;
	NEW(bitstream);

	bitstream->length = length;
	bitstream->stream = calloc(sizeof(char), bitstream->length);
	bitstream->position = 0;
	bitstream->offset = 0;
	return bitstream;
}

void bitstream_insert(B stream, unsigned char sequence, unsigned int bitlength) {
	assert(bitlength <= 8 && bitlength >= 1);
	assert(stream->offset < 8 && stream->offset >= 0);
#ifdef FIXED_SIZE
	assert(stream->position < stream->length);
#endif

	unsigned char first_byte = ((sequence & bit_masks[bitlength]) << stream->offset) & ~bit_masks[stream->offset];
	unsigned char second_byte = 0;
	stream->stream[stream->position] |= first_byte;

	if(stream->offset + bitlength >= 8) {

		second_byte =  (sequence >> (stream->offset + bitlength - 8)) & bit_masks[(stream->offset + bitlength - 8)];


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
		stream->stream[stream->position] |= second_byte;
		}
#endif
	}
	stream->offset = (stream->offset + bitlength) % 8;

#ifdef DEBUG
	printf("After inserting %p, offset %u, position %u, length %u\n",
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
	sequence = to_big_endian(sequence);

	unsigned char first_byte = ((sequence & bit_masks[bitlength]) << stream->offset) & ~bit_masks[stream->offset];
	unsigned char second_byte = 0;
	stream->stream[stream->position] |= first_byte;

	if(stream->offset + bitlength >= 8) {

		second_byte =  (sequence >> (stream->offset + bitlength - 8)) & bit_masks[(stream->offset + bitlength - 8)];


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
		stream->stream[stream->position] |= second_byte;
		}
#endif
	}
	stream->offset = (stream->offset + bitlength) % 8;

#ifdef DEBUG
	printf("After inserting %p, offset %u, position %u, length %u\n",
			sequence, stream->offset, stream->position, stream->length);
#endif
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
	}
}
