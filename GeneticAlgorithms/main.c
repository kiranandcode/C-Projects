#include "classifier.h"

#include <stdio.h>

int main() {

	bitstring_B stream = bitstring_generate("0110001110");
	pattern_B pattern =    pattern_generate("0###001110");
	
	bitstring_print(stream);
	printf("\n");
	pattern_print(pattern);
	printf("\n");
	if(pattern_matches(pattern, stream, NULL)) {
		printf("matches\n");
	} else {
		printf("does not match\n");
	}
}
