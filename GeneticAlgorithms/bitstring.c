#include "bitstring.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define B bitstring_B

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


static unsigned char and(unsigned char a, unsigned char b) {
	return a & b;
}

static unsigned char or(unsigned char a, unsigned char b) {
	return a | b;
}


static unsigned char xor(unsigned char a, unsigned char b) {
	return a ^ b;
}


struct B {
	unsigned char *stream;
	unsigned int length;   /* byte length in bitstream */
	unsigned int bits;     /* number of bits usable in stream[length-1]; */
	/* i.e bitlength = (length-1)*8 + bits; */
};

B bitstring_new(unsigned int bitlength) {
	assert(bitlength);

	unsigned int bits = bitlength %8;
	unsigned int length = bits ? bitlength/8 + 1 : bitlength/8;
	bits = bits ? bits : 8;

	B result;
	result = malloc(sizeof(*result));
	assert(result);
	result->length = length;
	result->bits   = bits;
	result->stream = calloc(sizeof(*result->stream),length);
	assert(result->stream);
	return result;
}

B bitstring_copy(B string) {
	B copy = bitstring_new(bitstring_get_bitlength(string));
	unsigned int i;
	for(i = 0; i < string->length; ++i) {
		copy->stream[i] = string->stream[i];
	}

	return copy;
}



void bitstring_set(B string,unsigned char sequence, unsigned int startbit, unsigned int sequence_length) {

	unsigned int bitlength = bitstring_get_bitlength(string);

	assert(startbit < bitlength
	&& startbit + sequence_length <= bitlength
	&& sequence_length <= 8);

	unsigned int first_byte_offset = startbit %8;
	unsigned int first_byte_index  = startbit/8;
	string->stream[first_byte_index] |= ((sequence & bit_masks[sequence_length]) << first_byte_offset) & 0xFF;
	if(startbit + sequence_length > 8) {
string->stream[first_byte_index+1] |= ((sequence >> (8 - first_byte_offset))) & bit_masks[sequence_length + first_byte_offset -8];
	}
}

B bitstring_map(B stringA, B stringB,
	unsigned char (*function) (unsigned char , unsigned char )) {

	assert(bitstring_get_bitlength(stringA) == bitstring_get_bitlength(stringB));
	B result = bitstring_new(bitstring_get_bitlength(stringA));
	int i;
	for(i = 0; (unsigned int)i < stringA->length; i++) {
		unsigned char value = 0;
		value = function(stringA->stream[i], stringB->stream[i]);
		if((unsigned int)i == (unsigned int)stringA->length-1)
			value &= bit_masks[stringA->bits];
		result->stream[i] = value;
	}	
	return result;
}

B bitstring_and(B stringA, B stringB) {
	return bitstring_map(stringA, stringB, and);
}

B bitstring_or(B stringA, B stringB) {
	return bitstring_map(stringA, stringB, or);
}

B bitstring_xor(B stringA, B stringB) {
	return bitstring_map(stringA, stringB, xor);
}

B bitstring_crossover(B stringA, B stringB, unsigned int pivot){
		assert(bitstring_get_bitlength(stringA) == bitstring_get_bitlength(stringB));
		assert(pivot < bitstring_get_bitlength(stringA));
	B result = bitstring_new(bitstring_get_bitlength(stringA));


	unsigned int toA = pivot / 8;
	unsigned int toAbits = pivot %8;
	unsigned int i;
	
	for(i = 0; i < result->length; i++) {
		if(i < toA) {
			result->stream[i] = stringA->stream[i];
		}
		else if(i == toA && toAbits){
			result->stream[i] |= stringA->stream[i] &bit_masks[toAbits];
			result->stream[i] |= stringB->stream[i] & ~bit_masks[toAbits];	
		} else {
			result->stream[i] = stringB->stream[i];
		}

	}
	return result;
}


void bitstring_print(B string) {
	unsigned int i,j;

	for(i = 0; i <string->length; ++i) {
		unsigned int max = 8;
		if(i == string->length-1)
			max = string->bits;
		unsigned char current_bytes = string->stream[i];
		for(j = 0; j < max; ++j) {
			if(1 & (current_bytes >> j))
				printf("1");
			else printf("0");
		}
//		if(i != (int)stream->length -1)
//			printf(",");
	}
}

unsigned int bitstring_get_bitlength(B string) {
	assert(string);
	return (string->length - 1) * 8 + string->bits;
}
