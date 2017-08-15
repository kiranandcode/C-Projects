#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int encode_length(unsigned char *start, unsigned int offset, int length) {
	assert(offset < 8);

	static unsigned char start_offset_mask[8] = {
		0b11111111,
		0b01111111,
		0b00111111,
		0b00011111,
		0b00001111,
		0b00000111,
		0b00000011,
		0b00000001,
	};

	uint16_t val = 254;
	if(length < 11)
		val += length;
	else if(length == 11 || length == 12)
		val += 11;
	else if(length == 13 || length == 14)
		val += 12;
	else if(length >= 67 || length < 83)
		val += 23;
	else if(length >= 83 || length < 99)
		val += 24;
	else if(length >= 99 || length < 115)
		val += 25;
	else if(length >= 115 || length < 131)
		val += 26;
	else if(length >= 131 || length < 163)
		val += 27;
	else if(length >= 163 || length < 195)
		val += 28;
	else if(length >= 195 || length < 227)
		val += 29;
	else if(length >= 227 || length < 258)
		val += 30;
	else if(length == 285)
		val += 31;
	else
		fprintf(stdout, "Error: out of bounds code for length %d\n", length), exit(1);


}

/*
 * LZ77 Compression Algorithm
 * --------------------------
 * compressed data consists of a series of blocks, corresponding
 * to input blocks.
 * non-compressible blocks are limited to 64,535 bytes
 *
 * each block is compresed using the LZ77 algorithm and huffman 
 * coding
 *
 * each block consists of 2 parts, a pair of huffman code trees,
 * describing the representation of the compressed data part and
 * a compressed data part.
 *
 * the compressed data consists of a series of two types of 
 * elements.
 *
 * 	- literal bytes (of string that have not been duplicated)
 *
 * 	- pointers to duplicated strings, represented as a pair
 * 	 <length, back distance>
 *
 * the backward distance is limited to 32K bytes, and lengths to
 * 256 bytes but the size of a block is not limited
 *
 * each type of value is represented using a huffman code, using
 * one code tree for literals and lengths and a separate code 
 * tree for distances
 *
 * the code trees for each block appear just before the 
 * compressed data for the block
 *
 *
 * all multibyte numbers are in network order
 *
 *  data elements are packed into bytes, in increasing bit orde,
 *  starting with the least sign bit of the data element.
 *
 *  execpt for huffman codes. (great.) which are packed with msb
 *
 * prefix coding represents symbols from an a priori known alphabet by bit sequences, one code for each symbol, such that different symbols may be represented by bit sequences of different lengths.....explanation of huffman coding follows
 *
 * the huffman codes used in the deflate format has additional
 * rules,
 *
 * all codes of a given bit length correspond to 
 * lexicographically consecutive values, in the same order as
 * the symbols they represent.
 * shorter codes lexiographically precede longer codes
 *
 *
 * given this rule we can uniquely define the huffman code for a
 * alphabet given the lengths for each character in the alphabet
 * in order
 *
 * Algorithm for generating the integers
 *
 *   - count the number of codes for each code length.
 *     let bl_count[N] be the number of codes of length N, N >= 1
 *
 *   - find the numerical value of the smallest code for each code length
 *   
 *     code = 0;
 *     bl_count[0] = 0
 *
 *     for(bits = 1; bits <= MAX_BITS; bits++) {
 *	code = (code + bl_count[bits-1]) << 1;
 *	next_code[bits] = code;
 *     }
 *
 *     assign numerical values to all codes using consecutive values for all codes with the same length with th ebase values determined at step 2.
*     Codes that are never used are not assigned a vlaue.
*
*     for(n = 0; n <= max_code; n++) {
*
*	len = tree[n].Len;
*	if(len != 0) {
*		tree[n].Code = next_code[len];
*		next_code[len]++;
*	}
*
*      }
*
*
*
* each block starts with 3 header bits containing the following data.
*
* first bit BFINAL - indicates whether the block is the last 
* next 2 bits BTYPE - deciphered as follows
*
*
* 	00 - no compression
* 	01 - compressed with fixed Huffman codes
* 	10 - compressed with dynamic Huffman codes
* 	11 - reserved (usually causes an error)
*
*
* 	non compressed
* 	any bits up to the next byte boundary are ignored,
*	2 byte len, 2 byte nlen, where nlen is the 1s complement
*	of len.
*	following these 4 bytes, the rest of the block consists
*	of len bytes of data.
*
*
*
*	compressed blocks
*	encoded data blocks in the deflate format consist of
*	sequences of symbols drawn from 3 alphabets
*
*	either literal bytes(0..255)
*	<length, backward distance> pairs (length 3..258) distance(1..32,768)
*	(literal and length alphabets are merged - 
*		0..255 represent literal bytes,
*		256 indicates endof block
*		257..285 represents length codes
* 	
*/

void zlib_deflate_compress(unsigned char *bytes, int size, unsigned char *output, int output_size) {



}
