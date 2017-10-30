#include "bits/pattern.h"
#include "collections/list.h"
#include "collections/set.h"
#include "classifier.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

bitstring_B expected_inputs[10];

void initialize_expected_inputs() {
	int i;
	for(i = 0; i < 10; i++) {
		expected_inputs[i] = bitstring_newbucketstring(28*28*8, 10, i);
	}
}

int read_and_train(FILE *fp, classifier_B classifier, int should_evolve) {
	
	int label;
	fscanf(fp, "%d,", &label);

	assert(label < 10 && label >= 0);
	bitstring_B expected = expected_inputs[label];

	bitstring_B input = bitstring_csv_fload(fp, 28 * 28);
	
	classifier_supervise(classifier, input, expected);

	if(should_evolve)
		classifier_evolve(classifier, 100, 24*24*8, 0.5);

	
	bitstring_delete(input);

	char c;
	if((c = getc(fp)) == EOF)
		return 0;
	else {
		ungetc(c,fp);
		return 1;
	}
}
int read_and_initialize(FILE *fp, classifier_B classifier, int should_evolve) {
	
	int label;
	fscanf(fp, "%d,", &label);

	assert(label < 10 && label >= 0);

	bitstring_B input = bitstring_csv_fload(fp, 28 * 28);
	
	classifier_input(classifier, input);

	if(should_evolve)
		classifier_evolve(classifier, 500, 24*24*8, 0.5);

	
	bitstring_delete(input);

	char c;
	if((c = getc(fp)) == EOF)
		return 0;
	else {
		ungetc(c,fp);
		return 1;
	}
}




int read_and_check(FILE *fp, classifier_B classifier, int *correct) {
	int label;
	fscanf(fp, "%d,", &label);

	assert(label < 10 && label >= 0);

	bitstring_B input = bitstring_csv_fload(fp, 28 * 28);
	
	classifier_input(classifier, input);

	bitstring_delete(input);

	bitstring_B output = classifier_getoutput(classifier);
	unsigned int guess = bitstring_asbuckets(output, 10);
	printf("Outputted %d for %d\n", guess, label);
	

	if(correct != NULL) {
	if(guess == (unsigned int) label) {
		*correct = 1;	
	} else
		*correct = 0;
	} else {
		bitstring_print(output);
	}

	char c;
	if((c = getc(fp)) == EOF)
		return 0;
	else {
		return 1;
	}
}



int main() {
	initialize_expected_inputs();
	/*
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
	*//*
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

	FILE *fp = fopen("mnist.csv", "r");

	classifier_B classifier =classifier_new(8*28*28,10000);
	int i;
	for(i =0 ; i< 1000; i++) {
		printf("initialization iteration %d\n", i);
		read_and_initialize(fp, classifier, i%41 == 0);
	}

	for(int j =0; j< 30; j++) {
		fclose(fp);
		fp = fopen("mnist.csv", "r");

			printf("training iteration %d\n", j);
		for(i =0 ; i< 900; i++) {
			read_and_train(fp, classifier, i%50 == 0);
		}

	}

	int correct = 0;
	for(i =0 ; i < 100; i++) {
		int current = 0;
		read_and_check(fp, classifier, &current);
		correct += current;
	}


	printf("Out of 100 test %d correct\n", correct);
	
//	read_and_check(fp, classifier, NULL);

}
