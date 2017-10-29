#ifndef BITSTRING_H
#define BITSTRING_H
#define B bitstring_B
#include <stdint.h>
#include <stdio.h>

struct B;
typedef struct B *B;

/*
 * @brief constructs a new bitstring
 * @param bitlength the length in bits of the bitstring
 * @return a 0-filled bitstring of the requested size
 */
B bitstring_new(unsigned int bitlength);
void bitstring_delete(B string);
B bitstring_copy(B string);
bitstring_B  bitstring_csv_fload(FILE *fp, unsigned int size);
int bitstring_eq(B stringA, B stringB);
B bitstring_random(unsigned int bitlength);
void bitstring_set(B string, unsigned char sequence, unsigned int startbit, unsigned int sequence_length);
void bitstring_bitset(B string, unsigned int bitposition);
void bitstring_bitclear(B string, unsigned int bitposition);
int  bitstring_bittest(B string, unsigned int bitposition);
B bitstring_map(B stringA, B stringB, unsigned char (*)(unsigned char, unsigned char));
B bitstring_and(B stringA, B stringB);
B bitstring_or(B stringA, B stringB);
B bitstring_xor(B stringA, B stringB);
B bitstring_crossover(B stringA, B stringB, unsigned int pivot);
unsigned int bitstring_bitcount(B string);
uint16_t bitstring_int(B string);

void bitstring_print(B string);
unsigned int bitstring_get_bitlength(B string);
bitstring_B bitstring_generate(char *str);
void bitstring_mutate(B string, unsigned int mutation_count, double mutation_probability);
unsigned int bitstring_simmilarity(B stringA, B stringB);
unsigned int bitstring_asbuckets(B stringA, unsigned int buckets);
bitstring_B bitstring_newbucketstring(unsigned int size, unsigned int buckets, unsigned int index);

#undef B
#endif
