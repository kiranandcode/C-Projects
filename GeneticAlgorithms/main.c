#include "bits/pattern.h"
#include "collections/list.h"
#include "collections/set.h"
#include "classifier.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
	bitstring_B stringA = bitstring_random(20);
        bitstring_B stringB = bitstring_random(20);
	bitstring_B stringC = bitstring_newbucketstring(20, 5, 2);

	printf("StringA: ");
	bitstring_print(stringA);
	printf("\n");
	printf("StringB: ");
	bitstring_print(stringB);
	printf("\n");
	printf("StringC: ");
	bitstring_print(stringC);
	printf("\n");

	int buckets = 5;
	printf("A as %d buckets: %u\n", buckets,  bitstring_asbuckets(stringA, buckets));
	printf("B as %d buckets: %u\n", buckets, bitstring_asbuckets(stringB, buckets));
	/*
	classifier_B classifier =classifier_new(8*28*28,10);
//	classifier_print(classifier);
    for(int i = 0; i < 1000; i++) {
        bitstring_B string = bitstring_random(8*28*28);
        classifier_input(classifier, string);
        //printf("Inputting String ");
        //bitstring_print(string);
        //printf("\n");
        //printf("Loop[%d]\n", i);
	classifier_evolve(classifier, 100, 50, 0.03);
    }
	classifier_printoutput(classifier);
	*/
}
