#include "random.h"

#include <time.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static int seed = -1;

void initialize_rand() {
	if(seed == -1.0) {
		seed = time(NULL);
		srand(seed);
	}
}


double random_normal(double low, double high) {
	assert(high > low);
	initialize_rand();
	// generates random values,
	// using marsaglia method
	double x,y,s,f;
	double range = high-low;

	do {
		x = (2.0*rand()/RAND_MAX) - 1.0;
		y = (2.0*rand()/RAND_MAX) - 1.0;
		s = x*x + y*y;
	} while(s >= 1 || s == 0);

	f = sqrt(-2.0 * log(s)/s);
	x = x*f;
	y = y*f;
	x = ((x)/5.0)*range + low;
	return x;
}

double random_range(double low, double high) {
	assert(high > low);
	initialize_rand();
	double range = high - low;
	double random = rand();
	double value = range*(random/RAND_MAX);
	return low + value;
}

double random_random() {
	initialize_rand();
	return rand();
}


void *random_choice(void *items, unsigned int size) {
	int index = random_range(0, size);	
	return items + index;
}

