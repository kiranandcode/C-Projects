#include "pattern.h"
#include "collections/list.h"
#include "core/random.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

struct pattern_B {
	bitstring_B string;
	bitstring_B mask;
};

pattern_B pattern_generate(char *str) {
	assert(str);
	pattern_B pattern;
	pattern = calloc(sizeof(*pattern),1);
	size_t size = strlen(str);
	pattern->mask = bitstring_new(size);
	pattern->string = bitstring_new(size);

	unsigned int i;
	for(i = 0; i < size; ++i){
		if(str[i] == '1') {
			bitstring_bitset(pattern->string,i);
			bitstring_bitset(pattern->mask, i);
		} else if(str[i] == '0') {
			bitstring_bitclear(pattern->string, i);
			bitstring_bitset(pattern->mask, i);
		}
	}
	return pattern;
}

void pattern_print(pattern_B pattern) {
	unsigned int i;
	unsigned int length = bitstring_get_bitlength(pattern->mask);

	for(i = 0; i < length; ++i) {
		if(!bitstring_bittest(pattern->mask, i)){
			printf("#");
		} else if(bitstring_bittest(pattern->string, i)) {
			printf("1");
		} else {
			printf("0");
		}
	}

}

unsigned int pattern_strength(pattern_B pattern) {
	return bitstring_bitcount(pattern->mask);
}

pattern_B pattern_random(unsigned int size) {
	assert(size);
	pattern_B pattern;
	pattern = calloc(sizeof(*pattern),1);
	assert(pattern);

	pattern->string = bitstring_random(size);
	pattern->mask   = bitstring_random(size);

	return pattern;
}

pattern_B pattern_crossover(pattern_B patternA, pattern_B patternB){
	assert(patternA);
	assert(patternB);
	assert(bitstring_get_bitlength(patternA->string)
		==
	       bitstring_get_bitlength(patternA->mask));
	
	assert(bitstring_get_bitlength(patternB->string)
		==
	       bitstring_get_bitlength(patternB->mask));

	assert(bitstring_get_bitlength(patternA->string) ==
			bitstring_get_bitlength(patternB->string));

	pattern_B pattern;
	pattern = calloc(sizeof(*pattern),1);
	pattern->string = bitstring_crossover(patternA->string,patternB->string, random_range(0, bitstring_get_bitlength(patternA->string)));
	pattern->mask = bitstring_crossover(patternA->mask,patternB->mask, random_range(0, bitstring_get_bitlength(patternA->mask)));
	return pattern;
}

int pattern_matches(pattern_B pattern, bitstring_B string, bitstring_B zeroref) {
	int delete_zero = 0;
	assert(bitstring_get_bitlength(pattern->string) 
			== 
	       bitstring_get_bitlength(string) &&
	(zeroref == NULL || bitstring_get_bitlength(zeroref) 
	  == 
	  bitstring_get_bitlength(pattern->string)));


	if(zeroref == NULL) {
		zeroref = bitstring_new(bitstring_get_bitlength(pattern->string));
		delete_zero = 1;
	}


	bitstring_B inpxorstr = bitstring_xor(pattern->string, string);

	bitstring_B ixsandmak = bitstring_and(inpxorstr, pattern->mask);


	int result = bitstring_eq(zeroref,ixsandmak);

	if(delete_zero)
		bitstring_delete(zeroref);
	bitstring_delete(inpxorstr);
	bitstring_delete(ixsandmak);


	return result;
}

pattern_B pattern_cover(bitstring_B string) {
	pattern_B result;
	result = malloc(sizeof(*result));
	assert(result);

	result->string = bitstring_copy(string);
	result->mask = bitstring_random(bitstring_get_bitlength(string));

	return result;
}


unsigned int pattern_hash(void *item) {
	assert(item);
	unsigned int result = 0;
	pattern_B pattern = item;


	unsigned int midpoint = (sizeof(unsigned int)*4);
	midpoint = (unsigned int)fmin((unsigned int)midpoint, (unsigned int)bitstring_get_bitlength(pattern->mask));
	unsigned int i;
	unsigned int offset;

	for(i = 0; i < midpoint; ++i){
		offset = i;
		if(bitstring_bittest(pattern->string, i)){
			result |= (1 << offset);
		}
	}
	for(i = 0; i < midpoint; ++i){
		offset = i + midpoint;
		if(bitstring_bittest(pattern->mask, i)){
			result |= (1 << offset);
		}
	}

	return result;
}
