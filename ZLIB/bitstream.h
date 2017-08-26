#ifndef BITSTREAM_H
#define BITSTREAM_H

// #define FIXED_SIZE
#ifdef BITSTREAM_INCREMENT 
#define STREAM_INCREMENT_SIZE BITSTREAM_INCREMENT 
#else
#define STREAM_INCREMENT_SIZE 1
#endif

#define B bitstream_B 

struct B;
typedef struct B *B;


B         bitstream_new(unsigned int length);
void      bitstream_delete(B bitstream);
B         bitstream_copy(B bitstream);
B         bitstream_concat(B bitstream_a, B bitstream_b);
// -------------------------------------------------------------
void      bitstream_insert(B stream, unsigned char sequence, unsigned int bitlength);
void      bitstream_big_endian_insert(B stream, unsigned char sequence, unsigned int bitlength);
// -------------------------------------------------------------
void      bitstream_trim(B bitstream);
void      bitstream_get_stream(B bitstream, unsigned char *stream, unsigned int length);
// -------------------------------------------------------------
void      print_char(unsigned char val);
void      bitstream_print(B stream);

#undef B
#endif
