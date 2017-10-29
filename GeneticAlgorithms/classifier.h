#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include "bits/bitstring.h"
#define B classifier_B 
struct B;
typedef struct B *B;

struct classifier_B *classifier_new(unsigned int size, unsigned int count);
void classifier_print(struct classifier_B *classifier);
void classifier_printoutput(classifier_B classifier);
void classifier_input(struct classifier_B *classifier, bitstring_B input);
void classifier_evolve(classifier_B classifer, unsigned int clipping_size, unsigned int mutation_count, double mutation_probability);

#undef B
#endif
