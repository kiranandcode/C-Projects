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
int pattern_matches(pattern_B pattern, bitstring_B string, bitstring_B zeroref);


#undef B
#endif
