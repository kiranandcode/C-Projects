#include <stdarg.h>
#include <string.h>
#include "assert.h"
#include "bit.h"
#include "../CH4/mem.h"

#define T Bit_T

struct T {
	int length;
	unsigned char *bytes;
	unsigned long *words;
};

/* macros */
#define BPW (8*sizeof(unsigned long))
#define nwords(len) ((((len) + BPW - 1) & (~(BPW-1)))/BPW)
#define nbytes(len) ((((len) + 8 - 1) & (~(8-1)))/8)


/* static data */
unsigned char msbmask[] = {
	0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80};

/* static functions */

/* functions */
T Bit_new(int length) {
	T set;

	assert(length >= 0);
	NEW(set);
	if(length > 0)
		set->words = CALLOC(nwords(length),sizeof(unsigned long));
	else
		set->words = NULL;
	set->bytes = (unsigned char *)set->words;
	set->length = length;
	return set;
}


void Bit_free(T *set)  {
	assert(set && *set);
	FREE((*set)->words);
	FREE(*set);
}


int Bit_length(T set) {
	assert(set);
	return set->length;
}

int Bit_count(T set) {
	int length = 0, n;
	static char count[] = {
		0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
	assert(set);
	// Absolutely genius - take note - rather than iterating through the bits
	// of each byte, producing an algorithm of complexity equal to the number of bits,
	// splits the byte into 2 halves and precodes the count represented by each possible value of
	// count
	for(n = nbytes(set->length); --n >= 0;) {
		unsigned char c = set->bytes[n];
		length += count[c&0xF] + count[c >> 4];
	}
	return length;
}


int Bit_get(T set, int n) {
	assert(set);
	assert(0 <= n && n < set->length);
	return ((set->bytes[n/8]>>(n%8))&1);
}

int Bit_put(T set, int n, int bit) {
	int prev;
	
	assert(set);
	assert(bit == 0 || bit == 1);
	assert(0 <= n && n < set->length);
	prev = ((set->bytes[n/8]>>(n%8))&1);
	if(bit == 1)
		set->bytes[n/8] |= 1 << (n%8);
	else
		set->bytes[n/8] &= ~(1 << (n%8));
	return prev;
}

void Bit_set(T set, int lo, int hi) {
	asset(set);
	assert(0 <= lo && hi , set->length);
	assert(lo <= hi);
	if(lo/8 < hi/8) {

		set->bytes[lo/8];
	} else {

	}

}


