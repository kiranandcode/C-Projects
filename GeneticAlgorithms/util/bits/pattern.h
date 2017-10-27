#ifndef PATTERN_H 
#define PATTERN_H
#include "bitstring.h"

struct pattern_B;
typedef struct pattern_B *pattern_B;

pattern_B pattern_generate(char *str);
void pattern_print(pattern_B pattern);
unsigned int pattern_strength(pattern_B pattern);
pattern_B pattern_random(unsigned int size);
pattern_B pattern_crossover(pattern_B patternA, pattern_B patternB);
pattern_B pattern_cover(bitstring_B string);
int pattern_matches(pattern_B pattern, bitstring_B string, bitstring_B zeroref);
void pattern_mutate(pattern_B pattern, unsigned int mutation_count, double mutation_probability);


#endif
