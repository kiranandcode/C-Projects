#ifndef ALLOC_H
#define ALLOC_H
#include "c.h"

#define NEW(p,a) ((p) = allocate(sizeof *(p), (a)))
#define NEWO(p,a) memset(NEW((p),(a)), 0,sizeof *(p))

extern void *allocate ARGS((unsigned long n, unsigned a));
extern void deallocate ARGS((unsigned a));

extern void *newarray ARGS((unsigned long m, unsigned long n, unsigned a));

#endif //ALLOC_H

