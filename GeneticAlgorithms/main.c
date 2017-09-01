#include <stdio.h>
#include "bitstring.h"

int main() {

	bitstring_B stringA = bitstring_new(10);
	bitstring_B stringB = bitstring_new(10);

	bitstring_set(stringA, 0xFF, 1, 8);
	bitstring_set(stringB, 0x55, 4, 6);

	bitstring_print(stringA);
	printf("\n");

	bitstring_print(stringB);
	printf("\n");

	bitstring_B string_result = bitstring_xor(stringA, stringB);

	bitstring_print(string_result);
	printf("\n");
}
