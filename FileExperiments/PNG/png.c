#include "global.h"
#include "pixel.h"
#include "hash.h"
#include "error.h"

#include <stdlib.h>
#include <math.h>
#include <netinet/in.h>


#define CHR4EQL(arr,ind,val) (arr)[(ind)]=(htonl(val)>>24)&0xFF;(arr)[(ind+1)]=(htonl(val)>>16)&0xFF;(arr)[(ind+2)]=(htonl(val)>>8)&0xFF;(arr)[(ind+3)]=(htonl(val))&0xFF;


/* transformations required
 *--------------------------
 * [alpha separation]
 * indexing
 * [RGB merging]
 * [alpha compaction]
 * sample depth scaling
 *
 */

char png_signature[] = {
	137,
	80,
	78,
	71,
	13,
	10,
	26,
	10
};

struct png_chunk {
	char length[4];
	char chunk_type[4];
	char *bytes;
	char CRC[4];
};


/* chunk_type semantics 
 * --------------------
 *  - first byte: 
 *  	0: critical
 *  	1: ancilliary
 *  - 2nd byte:
 *  	0: public (a chunk defined in the standard)
 *  	1: private(unregistered custom chunk)
 *  - 3d byte:
 *  	0: only acceptable value
 *  - 4th byte:
 *  	0: unsafe to copy
 *  	1: safe to copy 
 * 
 */

struct png_chunk generate_IHDR(int width, int height,
				int bit_depth, int color_type,
				int compression_method,
				int filter_method,
				int interlace_method) {
	struct png_chunk chunk;

	chunk.length[0] = (htonl(13) >> 24) & 0xFF;
	chunk.length[1] = (htonl(13) >> 16) & 0xFF;
	chunk.length[2] = (htonl(13) >> 8) & 0xFF;
	chunk.length[3] = (htonl(13) & 0xFF);

	chunk.chunk_type[0] = 0x49;
	chunk.chunk_type[1] = 0x48;
	chunk.chunk_type[2] = 0x44;
	chunk.chunk_type[3] = 0x52;

	chunk.bytes = malloc(sizeof(char)*13);

	/*nk.chunk_type = 0x49484452;
  

	 * (n >> 24) & 0xFF
	 * (n >> 16) & 0xFF
	 * (n >> 8) & 0xFF
	 * (n & 0xFF)
	 */

	// width
	chunk.bytes[0] = (htonl(width) >> 24) & 0xFF;
	chunk.bytes[1] = (htonl(width) >> 16) & 0xFF;
	chunk.bytes[2] = (htonl(width) >> 8) & 0xFF;
	chunk.bytes[3] = (htonl(width) & 0xFF);

	
	// height
	chunk.bytes[4] = (htonl(height) >> 24) & 0xFF;
	chunk.bytes[5] = (htonl(height) >> 16) & 0xFF;
	chunk.bytes[6] = (htonl(height) >> 8) & 0xFF;
	chunk.bytes[7] = (htonl(height) & 0xFF);

	// bit_depth - single byte integer, no bits per  sample
	// if indexed only 1,2,4,8 supported
	// if truecolor only 8 and 16 supported
	chunk.bytes[8] = bit_depth;

	// color_type
	// defines the type
	// 0   - greyscale (each pixel is greyscale) 1,2,4,16
	// 2   - truecolor (each pixel is R,G,B)     8,16
	// 3   - indexed color (each pixel is index) 1,2,4,8
	// 4   - greyscale with alpa (grayscl,alpha) 8,16
	// 6   - truecolor w.alpha                   8,16
	chunk.bytes[9] = color_type;

	// compression_method -  only supported 0
	chunk.bytes[10] = compression_method;

	// filter method - filter method 0 only supported
	chunk.bytes[11] = filter_method;

	// interlace method 0 for no, 1 for Adam7 interlace
	chunk.bytes[12] = interlace_method;


	// TODO: generate Checkbyte


	return chunk;

}

struct png_chunk generate_PLTE(P_int *palette, int size) {
	struct png_chunk chunk;
	int pltesize = (size/4)*3;
	CHR4EQL(chunk.length,0,pltesize)
	
	chunk.chunk_type[0] = 0x50;
	chunk.chunk_type[1] = 0x4C;
	chunk.chunk_type[2] = 0x54;
	chunk.chunk_type[3] = 0x45;

	chunk.bytes = malloc(sizeof(char)*pltesize);

	int i;
	for(i = 0; i<size/4; i++) {
		chunk.bytes[(i*3)+0] = palette[(i*4)+0];
		chunk.bytes[(i*3)+1] = palette[(i*4)+1];
		chunk.bytes[(i*3)+2] = palette[(i*4)+2];
	}


	// TODO: calculate checkbyte

	return chunk;
}

struct png_chunk generate_IDAT(char *data, int size) {

	struct png_chunk chunk;

	CHR4EQL(chunk.length, 0, size);
	
	chunk.chunk_type[0] = 73;
	chunk.chunk_type[1] = 68;
	chunk.chunk_type[2] = 65;
	chunk.chunk_type[3] = 84;

	chunk.bytes = malloc(sizeof(char) * size);

	int i;
	for(i = 0; i < size; i++) {
		chunk.bytes[i] = data[i];
	}
	
	// TODO: calculate CRC

	return chunk;
}

struct png_chunk generate_IEND() {

	struct png_chunk chunk;

	CHR4EQL(chunk.length,0,0);
	
	chunk.chunk_type[0] = 73;
	chunk.chunk_type[1] = 69;
	chunk.chunk_type[2] = 78;
	chunk.chunk_type[3] = 68;

	chunk.bytes = NULL;

	// TODO: calculate CRC bytes


	return chunk;
}

void alpha_separation(struct pixel *data, int w, int h) {

	int i;
	P_int alpha = ALPHA_MAX;

	for(i = 0; i< w*h; ++i) {
		if(data[i].alpha != alpha) return;
	}

	// if reached, all alpha values are the maximum 
	
	for(i = 0; i< w*h; ++i) {
		data[i].alpha = ALPHA_IGNORE;
	}

}


int indexing(struct pixel *data, int w, int h, P_int *palette) {
	int i;
	int seen = 0;
	struct pixel *store = malloc(sizeof(struct pixel));

	struct h_hash_table *table = h_init_table(
					pixel_compare,
					pixel_delete,
					pixel_hash);

	for(i = 0; i < w * h; i++){
		store->r = data[i].r;
		store->g = data[i].g;
		store->b = data[i].b;
		store->alpha = data[i].alpha;

		if(h_hash_insert(table, store)){
			seen++;
			if(seen > 256) {

				h_delete_table(table);
				return 0;
			}
			store = malloc(sizeof(struct pixel));
		}

	}

	void **entries = malloc(sizeof(void *) * seen);
	h_hash_get_entries(table, entries, seen);


	int row = 0;

	for(i = 0; i<seen; i++) {
		struct pixel* pixel = (struct pixel *) entries[i];
		palette[(row*4)+0] = pixel->r;
		palette[(row*4)+1] = pixel->g;
		palette[(row*4)+2] = pixel->b;
		palette[(row*4)+3] = pixel->alpha;
		row++;
	}

	free(entries);
	free(store);
	h_delete_table(table);
	return row * 4;
}

void encode(struct pixel *data, unsigned char *output, int w, int h, int bit_depth, int image_type) {
	// for each pixel in data,
	// 	depending on the bit_depth
	// 		translate the rgba
	// 		insert into bitdepth pixels
	unsigned char *output_iterator = output;

	int i,j;
	char mask = 1;
	char ref_mask = 1;
	
	int use_mask = 1;
	int mask_increment = 1;

	switch(bit_depth){
		case 1:
			mask = 0x40;
			mask_increment = 1;
			break;
		case 2:
			mask = 0xC0;
			mask_increment = 2;
			break;
		case 4:
			mask = 0xF0;
			mask_increment = 4;
			break;
		case 8:
			use_mask = 0;
			break;
		case 16:
			use_mask = 0;
			break;
	}
	ref_mask = mask;

	for(j = 0; j < h; ++j) {
		for(i = 0; i < w; ++i) {
			struct pixel *current = data + (j*w) + i;
			
			if(use_mask){
				switch(image_type){ 
					case 0:
						*output_iterator |= (((unsigned int)current->r) & mask);
						mask >>= mask_increment;
						if(!mask) {
							mask = ref_mask;
							output_iterator++;
						}
						break;
					case 2:
						*output_iterator |= (((unsigned int)current->r) & mask);
						mask >>= mask_increment;
						if(!mask) {
							mask = ref_mask;
							output_iterator++;
						}

						*output_iterator |= (((unsigned int)current->g) & mask);
						mask >>= mask_increment;
						if(!mask) {
							mask = ref_mask;
							output_iterator++;
						}

						*output_iterator |= (((unsigned int)current->b) & mask);
						mask >>= mask_increment;
						if(!mask) {
							mask = ref_mask;
							output_iterator++;
						}


						break;
					case 3:
						*output_iterator |= (((unsigned int)current->r) & mask);
						mask >>= mask_increment;
						if(!mask) {
							mask = ref_mask;
							output_iterator++;
						}

						break;
					case 4:
						error("Greyscale with alpha(4) mode is not supported for sub byte bit_depths(<8)");
					case 6:
						error("Truecolour with alpha(6) mode is not supported for sub byte bit_depths (<8)");
						break;
				}
			}
			else {
				switch(image_type) {
					case 0:
						if(bit_depth == 8) {
							*output_iterator++ = (unsigned int)current->r;
						} else {
							*output_iterator++ = (htons(current->r) >> 8) & 0xFF;
							*output_iterator++ = (htons(current->r)) & 0xFF;
						}
						break;
					case 2:
						if(bit_depth == 8) {
							*output_iterator++ = (unsigned int)current->r;
							*output_iterator++ = (unsigned int)current->g;
							*output_iterator++ = (unsigned int)current->b;
						} else {
							*output_iterator++ = (htons(current->r) >> 8) & 0xFF;
							*output_iterator++ = (htons(current->r)) & 0xFF;
							*output_iterator++ = (htons(current->g) >> 8) & 0xFF;
							*output_iterator++ = (htons(current->g)) & 0xFF;

							*output_iterator++ = (htons(current->b) >> 8) & 0xFF;
							*output_iterator++ = (htons(current->b)) & 0xFF;
						}
						break;
					case 3:
						if(bit_depth == 8) {
							*output_iterator++ = (unsigned int)current->r;
						} else {
							error("Indexed Color does not support a bit_depth of 16");
						}
						break;
					case 4:
						if(bit_depth == 8) {

							*output_iterator++ = (unsigned int)current->r;
							*output_iterator++ = (unsigned int)current->alpha;

						} else {
							*output_iterator++ = (htons(current->r) >> 8) & 0xFF;
							*output_iterator++ = (htons(current->r)) & 0xFF;
							*output_iterator++ = (htons(current->alpha) >> 8) & 0xFF;
							*output_iterator++ = (htons(current->alpha)) & 0xFF;
						}

						break;
					case 6:
						if(bit_depth == 8) {


							*output_iterator++ = (unsigned int)current->r;
							*output_iterator++ = (unsigned int)current->g;
							*output_iterator++ = (unsigned int)current->b;
							*output_iterator++ = (unsigned int)current->alpha;
						} else {
							*output_iterator++ = (htons(current->r) >> 8) & 0xFF;
							*output_iterator++ = (htons(current->r)) & 0xFF;
							*output_iterator++ = (htons(current->g) >> 8) & 0xFF;
							*output_iterator++ = (htons(current->g)) & 0xFF;
							*output_iterator++ = (htons(current->b) >> 8) & 0xFF;
							*output_iterator++ = (htons(current->b)) & 0xFF;
							*output_iterator++ = (htons(current->alpha) >> 8) & 0xFF;
							*output_iterator++ = (htons(current->alpha)) & 0xFF;
						}
						break;
				}
			}

		}

	}

}

unsigned char filter_byte(unsigned char x, unsigned char a, unsigned char b, unsigned char c, int filter_method) {
	switch(filter_method){
		case 0:{
			       return x;
		       break;
		       }
	      	case 1:{
			       return x - a;
		       break;
		       }
		case 2:{
			       return x - b;
		       break;
		       }
		case 3:{
			       return x - floor((long)(a+b)/2);
		       break;
		       }
		case 4:{
			       long p = a + b - c;
			       long pa = labs(p-a);
			       long pb = labs(p-b);
			       long pc = labs(p-c);
			       if(pa <= pb && pa <=pc) return a;
			       else if(pb <= pc) return b;
			       else return c;
		       break;
		       }
		default:
		       error("Invalid filter method");
		       return 0;
	}
}

void filter(unsigned char *data, unsigned char*output, int w, int h, int bit_depth, int image_type, int filter_method) {

	// 0   - greyscale (each pixel is greyscale) 1,2,4,16
	// 2   - truecolor (each pixel is R,G,B)     8,16
	// 3   - indexed color (each pixel is index) 1,2,4,8
	// 4   - greyscale with alpa (grayscl,alpha) 8,16
	// 6   - truecolor w.alpha                   8,16

	unsigned char *output_iterator = output;
	int byte_w;
	int pixel_byte_width;
	
	if(bit_depth < 8) {
		switch(bit_depth){
			case 1:
				switch(image_type) {
					case 0:
						// in terms of bytes, each every 8 pixels counts as 1
						byte_w = w/8;
						break;
					case 2:
						error("True color does not support <8bit depth");
						break;
					case 3:
						// in terms of bytes every 8 pixels counts as 1
						byte_w = w/8;
						break;
					case 4:
						error("Greyscale with alpha does not support <8bit depth");
						break;
					case 6:
						error("Truecolor w.alpha does not support <8bit depth");
						break;
				}
				break;
			case 2:
				switch(image_type) {
					case 0:
						// in terms of bytes, every 4 pixels counts as 1
						byte_w = w/4;
						break;
					case 2:
						error("True color does not support <8bit depth");
						break;
					case 3:
						// in terms of bytes, every 4 pixels counts as 1
						byte_w = w/4;
						break;
					case 4:
						error("Greyscale with alpha does not support <8bit depth");
						break;
					case 6:
						error("Truecolor w.alpha does not support <8bit depth");
						break;

				}
				break;
			case 4:
				switch(image_type) {
					case 0:
						// in terms of bytes, every 2 pixels counts as 1
						byte_w = w/2;
						break;
					case 2:
						error("True color does not support <8bit depth");
						break;
					case 3:
						// in terms of bytes, every 2 pixels counts as 1
						byte_w = w/2;
						break;
					case 4:
						error("Greyscale with alpha does not support <8bit depth");
						break;
					case 6:
						error("Truecolor w.alpha does not support <8bit depth");
						break;

				}
				break;
		}

	} else {
		byte_w = w;
		switch(image_type) {

			case 0:
				pixel_byte_width = 1;
				break;
			case 2:
				pixel_byte_width = 3;
				break;
			case 3:
				pixel_byte_width = 3;
				break;
			case 4:
				pixel_byte_width = 2;
				break;
			case 6:
				pixel_byte_width = 4;
				break;
		}

		if(bit_depth == 16)
			pixel_byte_width *= 2;
	}


	int i, j;

	for(j = 0; j < h; ++j) {
		*output_iterator = filter_method;

		for(i = 0; i < byte_w; ++i) {

			if(bit_depth < 8) {

				unsigned char x= *(data + (j*byte_w) + i);
				unsigned char a = (i > 0) ? *(data + (j*byte_w) + i-1) : 0;
				unsigned char b = (j > 0) ? *(data + ((j-1)*byte_w) + i) : 0;
				unsigned char c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w) + i-1) : 0;
				unsigned char result = filter_byte(x,a,b,c,filter_method);

				*output_iterator++ = result;


			} else {
				if(bit_depth == 8) {
					switch(image_type){
						case 0: {
							unsigned char x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width);
							unsigned char a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0;
							unsigned char b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width)  : 0;
							unsigned char c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0;
							unsigned char result = filter_byte(x,a,b,c,filter_method);
							*output_iterator++ = result;
							break;
							}
						case 2:{
							unsigned char x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width);
							unsigned char a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0;
							unsigned char b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width)  : 0;
							unsigned char c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0;
							unsigned char result_r = filter_byte(x,a,b,c,filter_method);

							
							x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width + 1);
							a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0;
							b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 1)  : 0;
							c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0;
							unsigned char result_g = filter_byte(x,a,b,c,filter_method);

							x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width + 2);
							a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 2) : 0;
							b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 2)  : 0;
							c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 2) : 0;
							unsigned char result_b = filter_byte(x,a,b,c,filter_method);

							*output_iterator++ = result_r;
							*output_iterator++ = result_g;
							*output_iterator++ = result_b;
							break;
						       }
						case 3: {
							unsigned char x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width);
							unsigned char a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0;
							unsigned char b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width)  : 0;
							unsigned char c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0;
							unsigned char result = filter_byte(x,a,b,c,filter_method);
							*output_iterator++ = result;
	
							break;
							}
						case 4:{
							unsigned char x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width);
							unsigned char a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0;
							unsigned char b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width)  : 0;
							unsigned char c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0;
							unsigned char result_g = filter_byte(x,a,b,c,filter_method);

							x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width + 1);
							a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0;
							b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 1)  : 0;
							c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0;
							unsigned char result_a = filter_byte(x,a,b,c,filter_method);

							*output_iterator++ = result_g;
							*output_iterator++ = result_a;
	
							break;
						       }
						case 6:{
							unsigned char x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width);
							unsigned char a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0;
							unsigned char b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width)  : 0;
							unsigned char c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0;
							unsigned char result_r = filter_byte(x,a,b,c,filter_method);

																
							x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width + 1);
							a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0;
							b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 1)  : 0;
							c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0;
							unsigned char result_g = filter_byte(x,a,b,c,filter_method);

							x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width + 2);
							a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 2) : 0;
							b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 2)  : 0;
							c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 2) : 0;
							unsigned char result_b = filter_byte(x,a,b,c,filter_method);
							x = *(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width + 3);
							a = (i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 3) : 0;
							b = (j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 3)  : 0;
							c = (i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 3) : 0;

							unsigned char result_a = filter_byte(x,a,b,c,filter_method);
							*output_iterator++ = result_r;
							*output_iterator++ = result_g;
							*output_iterator++ = result_b;
							*output_iterator++ = result_a;
							break;
						       }
					}

				} else {
					switch(image_type){
						case 0: {
							unsigned char x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width));
							unsigned char a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0);
							unsigned char b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width)  : 0);
							unsigned char c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0);

							unsigned char result_0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 1);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 1)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0);

							unsigned char result_1 = htons(filter_byte(x,a,b,c,filter_method));
							*output_iterator++ = result_0;
							*output_iterator++ = result_1;
							break;
							}
						case 2:{
							unsigned char x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width));
							unsigned char a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0);
							unsigned char b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width)  : 0);
							unsigned char c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0);

							unsigned char result_r0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 1);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 1)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0);

							unsigned char result_r1 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width + 2));
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 2) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 2)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 2) : 0);

							unsigned char result_g0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 3);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 3) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 3)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 3) : 0);

							unsigned char result_g1 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width + 4));
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 4) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 4)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 4) : 0);

							unsigned char result_b0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 5);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 5) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 5)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 5) : 0);

							unsigned char result_b1 = htons(filter_byte(x,a,b,c,filter_method));

							*output_iterator++ = result_r0;
							*output_iterator++ = result_r1;

							*output_iterator++ = result_g0;
							*output_iterator++ = result_g1;

							*output_iterator++ = result_b0;
							*output_iterator++ = result_b1;
							
							break;
						       }
						case 3: {
							unsigned char x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width));
							unsigned char a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0);
							unsigned char b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width)  : 0);
							unsigned char c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0);

							unsigned char result_0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 1);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 1)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0);

							unsigned char result_1 = htons(filter_byte(x,a,b,c,filter_method));
							*output_iterator++ = result_0;
							*output_iterator++ = result_1;

							break;
							}
						case 4:{
							unsigned char x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width));
							unsigned char a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0);
							unsigned char b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width)  : 0);
							unsigned char c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0);

							unsigned char result_g0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 1);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 1)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0);

							unsigned char result_g1 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 2);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 2) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 2)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 2) : 0);

							unsigned char result_a0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 3);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 3) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 3)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 3) : 0);

							unsigned char result_a1 = htons(filter_byte(x,a,b,c,filter_method));
							*output_iterator++ = result_g0;
							*output_iterator++ = result_g1;
							*output_iterator++ = result_a0;
							*output_iterator++ = result_a1;

							break;
						       }
						case 6:{
							unsigned char x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width));
							unsigned char a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0);
							unsigned char b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width)  : 0);
							unsigned char c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width) : 0);

							unsigned char result_r0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 1);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 1)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 1) : 0);

							unsigned char result_r1 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width + 2));
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 2) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 2)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 2) : 0);

							unsigned char result_g0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 3);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 3) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 3)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 3) : 0);

							unsigned char result_g1 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width + 4));
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 4) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 4)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 4) : 0);

							unsigned char result_b0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 5);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 5) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 5)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 5) : 0);

							unsigned char result_b1 = htons(filter_byte(x,a,b,c,filter_method));

							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 6);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 6) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 6)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 6) : 0);

							unsigned char result_a0 = htons(filter_byte(x,a,b,c,filter_method));
							x =ntohs(*(data + (j*byte_w*pixel_byte_width) + i*pixel_byte_width) + 7);
							a =ntohs((i > 0) ? *(data + (j*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 7) : 0);
							b =ntohs((j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + i*pixel_byte_width + 7)  : 0);
							c =ntohs((i > 0 && j > 0) ? *(data + ((j-1)*byte_w*pixel_byte_width) + (i-1)*pixel_byte_width + 7) : 0);

							unsigned char result_a1 = htons(filter_byte(x,a,b,c,filter_method));



							*output_iterator++ = result_r0;
							*output_iterator++ = result_r1;

							*output_iterator++ = result_g0;
							*output_iterator++ = result_g1;

							*output_iterator++ = result_b0;
							*output_iterator++ = result_b1;
							
							*output_iterator++ = result_a0;
							*output_iterator++ = result_a1;

							break;
						       }
					}


				}
			}	
			

		}


	}	




}


void zlib_compress(unsigned char *data, unsigned char *output, int bytes){
	unsigned int LZ77_window_size;
	unsigned int FCHECK;
	unsigned int FDICT;
	unsigned int FLEVEL;

	// specify compression mode 8, LZ77 window of 32768
	// if size <16384 then window size may be rounded up
	unsigned char compression_method_flags = 0x8;
	compression_method_flags |= ( (unsigned int)log2(LZ77_window_size-7) << 4);
	unsigned char additional_flags = 0;
	additional_flags |= (FCHECK & 0x1F);
	additional_flags |= ((FDICT << 5) & 0x20);
	additional_flags |= ((FLEVEL <<6) & 0xA0);

	unsigned char check_value[4];


}
