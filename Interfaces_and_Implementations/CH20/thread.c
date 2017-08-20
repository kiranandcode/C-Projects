#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include </usr/include/signal.h>
#include <sys/time.h>
#include "assert.h"
#include "thread.h"

void _MONITOR(void) {}
extern void _ENDMONITOR(void);

#define T Thread_T
#define isempty(q) ((q) == NULL)


struct T {
	unsigned long *sp;
	T link;
	T *inqueue;
};


static void put(T t, T *q) {
	assert(t);
	assert(t->inqueue == NULL && t->link == NULL);
	if(*q) {
		t->link = (*q)->link;
		(*q)->link = t;
	} else 
		t->link = t;
	*q = t;
	t->inqueue = q;
}

static T get(T *q) {
	T t;
	assert(!isempty(*q));
	t = (*q)->link;
	if(t == *q)
		*q = NULL;
	else
		(*q)->link = t->link;
	assert(t->inqueue == q);
	t->link = NULL;
	t->inqueue = NULL;
	return t;
}

static void delete(T t, T *q) {
	T p;

	assert(t->link && t->inqueue == q);
	assert(!isempty(*q));
	for(p = *q; p->link != t; p = p->link)
		;
	if(p == t)
		*q = NULL;
	else {
		p->link = t->link;
		if(*q == t)
			*q = p;
	}
	t->link = NULL;
	t->inqueue = NULL;
}


}


static T ready = NULL;


#undef T
