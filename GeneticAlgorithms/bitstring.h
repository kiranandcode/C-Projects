#ifndef BITSTRING_H
#define BITSTRING_H
#define B bitstring_B
struct B;
typedef struct B *B;

/*
 * @brief constructs a new bitstring
 * @param bitlength the length in bits of the bitstring
 * @return a 0-filled bitstring of the requested size
 */
B bitstring_new(unsigned int bitlength);
B bitstring_copy(B string);
void bitstring_set(B string, unsigned char sequence, unsigned int startbit, unsigned int sequence_length);
B bitstring_map(B stringA, B stringB, unsigned char (*)(unsigned char, unsigned char));
B bitstring_and(B stringA, B stringB);
B bitstring_or(B stringA, B stringB);
B bitstring_xor(B stringA, B stringB);
B bitstring_crossover(B stringA, B stringB, unsigned int pivot);

void bitstring_print(B string);
unsigned int bitstring_get_bitlength(B string);

#undef B
#endif
