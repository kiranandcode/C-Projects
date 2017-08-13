#include "global.h"
#include "pixel.h"

#include <stdlib.h>

int pixel_compare(void *a,void *b) {

	struct pixel *pixelA = (struct pixel *)a;
	struct pixel *pixelB = (struct pixel *)b;

	
	return (pixelA->r == pixelB->r
	     && pixelA->g == pixelB->g
	     && pixelA->b == pixelB->b
	     && pixelA->alpha == pixelA->alpha);

}
void pixel_delete(void *a){
	free(a);
}
int pixel_hash(void *a, int size){
	struct pixel *pixelA = (struct pixel *)a;

	int hash = 1;
	hash *= (pixelA->r + 1);
	hash %= size;
	hash *= (pixelA->g + 1);
	hash %= size;
	hash *= (pixelA->b + 1);
	hash %= size;
	if(pixelA->alpha != ALPHA_IGNORE) {
		hash *= (pixelA->alpha + 1);
		hash %= size;
	}

	return hash;
}

void pixel_indexed_convert(struct pixel *data, P_int *output, int w, int h, P_int *palette, int size) {

	int i;
	for(i = 0; i<w*h; ++i) {

		struct pixel *pixel = data + i;

		int row = 0;
		for(row = 0; row < size; row+= 4) {
			if(
		palette[(row * 4)+0] == pixel->r
	     && palette[(row * 4)+1] == pixel->g
	     && palette[(row * 4)+2] == pixel->b
	     && palette[(row * 4)+3] == pixel->alpha) break;

		}
		output[i] = row;

	}


}
