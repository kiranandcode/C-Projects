#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include "bitstring.h"
#define B classifier_B 

struct pattern_B;
struct B;
typedef struct B *B;
typedef struct pattern_B *pattern_B;

pattern_B pattern_generate(char *str);
void pattern_print(pattern_B pattern);
pattern_B pattern_random(unsigned int size);
pattern_B pattern_crossover(pattern_B patternA, pattern_B patternB);
int pattern_matches(pattern_B pattern, bitstring_B string, bitstring_B zeroref);


#undef B
#endif
