#ifndef PIXEL_H
#define PIXEL_H
#include "global.h"

// Truecolor pixel
struct pixel {
	P_int r;
	P_int g;
	P_int b;
	P_int alpha;
};


int pixel_compare(void *,void *);
void pixel_delete(void *);
int pixel_hash(void *, int);


#endif //PIXEL_H
