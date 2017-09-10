#ifndef SIMPLE_GA
#define SIMPLE_GA
#include "bitstring.h"

bitstring_B haploid_genetic_algorithm(unsigned int population_size, unsigned int individual_size, unsigned int iterations, double fitness(bitstring_B string),int print_result);

#endif
