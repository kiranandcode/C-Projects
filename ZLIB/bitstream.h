#ifndef BITSTREAM_H
#define BITSTREAM_H

// #define FIXED_SIZE
#ifdef BITSTREAM_INCREMENT 
#define STREAM_INCREMENT_SIZE BITSTREAM_INCREMENT 
#else
#define STREAM_INCREMENT_SIZE 10
#endif

#define B bitstream_B 

struct B;
typedef struct B *B;


B bitstream_new(unsigned int length);
void bitstream_insert(B stream, unsigned char sequence, unsigned int bitlength);
void bitstream_big_endian_insert(B stream, unsigned char sequence, unsigned int bitlength);


void bitstream_print(B stream);

#undef B
#endif
