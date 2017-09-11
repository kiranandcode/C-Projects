#include "classifier.h"
#include "list.h"
#include "random.h"

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
	unsigned char on_msg_board;
	unsigned char output;
	double strength;
};

struct classifier_B {
	unsigned int bitstring_size;
	bitstring_B zeroref;
	list_L elements;
	list_L message_board;
//	struct classifier_element_B *elements;
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


void classifier_input(classifier_B classifier, bitstring_B input) {

	assert(classifier->bitstring_size == bitstring_get_bitlength(input));
	
	list_L matching = classifier->message_board;
	struct L_iterator iter = list_iterator(classifier->elements);
	// collects matching patterns into messageboard
	while(list_iteratorhasnext(&iter)) {
		struct classifier_element_B *elem = list_iteratornext(&iter);
		if(pattern_matches(elem->pattern, input, classifier->zeroref) && !elem->on_msg_board){
			list_push(matching, elem);
		}
	}


}

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
