#include "classifier.h"

#include <assert.h>
#include <stdlib.h>

typedef struct pattern_B *pattern_B;

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

int pattern_matches(pattern_B pattern, bitstring_B string, bitstring_B zeroref) {
	int delete_zero = 0;
	assert(bitstring_get_bitlength(pattern->string) 
			== 
	       bitstring_get_bitlength(string) &&
	(zeroref == NULL || bitstring_get_bitlength(zeroref) 
	  == 
	  bitstring_get_bitlength(pattern->string->length)));


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
