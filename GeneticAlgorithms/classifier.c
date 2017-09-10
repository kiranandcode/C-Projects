#include "classifier.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct pattern_B {
	bitstring_B string;
	bitstring_B mask;
};

struct classifier_element_B {
	pattern_B pattern;
	bitstring_B result;
};

struct classifier_B {
	unsigned int classifiers;
	unsigned int input_bitstring_size;
	unsigned int output_bitstring_size;
	struct classifier_element_B *elements;
};

// input : 10111111111111
// result: 1####1####0###
//         ----------e---
//                   error
//
// string: 10000100010010
//   mask: 10000100001000 
// result: 1####1####0###
//
// inp&st: 10000100010010
//
// inp^st: 00111011101101
//   mask: 10000100001000 
//in^st&m: 00000000001000

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
