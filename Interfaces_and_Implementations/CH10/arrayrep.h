#ifndef ARRAYREP_H
#define ARRAYREP_H

#define T Array_T

// ArrayRep reveals the internals of an array
struct T  {
	int length;
	int size;
	char *array;
};

extern void ArrayRep_init(T array, int length, int size,void *ary);

#undef T
#endif
