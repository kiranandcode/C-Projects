#ifndef RANDOM_H
#define RANDOM_H
#define RANOM_CHOICE(arr, sz, type) \
	(type)*(random_choice(arr,sz))

double random_normal(double low, double high);
double random_range(double low, double high);
double random_random();
void *random_choice(void *items, unsigned int size);

#endif //RANDOM_H
