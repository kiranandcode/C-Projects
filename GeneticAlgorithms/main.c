#include "bits/pattern.h"
#include "collections/list.h"
#include "collections/set.h"
#include "classifier.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
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
}
