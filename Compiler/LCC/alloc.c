#include "alloc.h"
#include <stdlib.h>

// -----------------------------------------------------
// types
// -----------------------------------------------------
struct block {
	struct block *next;
	char *limit;
	char *avail;
};

// a type most likely to hold the required alignment
union align {
	long l;
	char *p;
	double d;
	int (*f) ARGS((void));
};

// used to ensure that pointers are aligned
union header {
	struct block b;
	union align a;
};

#ifdef PURIFY
// ------------------------------------------------------
// debugging implementation
// ------------------------------------------------------
#else
// ------------------------------------------------------
// data 
// ------------------------------------------------------

static struct block
	first[] = { { NULL, NULL, NULL }, { NULL, NULL, NULL }, { NULL, NULL, NULL } },
       *arena[] = { &first[0], &first[1], &first[2] };

static struct block *freeblocks;

// ------------------------------------------------------
// functions
// ------------------------------------------------------
void *allocate(n, a) unsigned long n; unsigned a; {

	struct block *ap;

	ap = arena[a];
	// the size we allocate is the nearest multiple of the
	// alignment minumum to our space requirements
	n = roundup(n, sizeof(union align));
	// while the ap requested doesn't have space
	while(ap->avail + n > ap->limit){
		// get a new block
		// if there are freeblocks
		if((ap->next = freeblocks) != NULL) {
			// retrieve the freeblock, and remove from list
			freeblocks = freeblocks->next;
			ap = ap->next;
		} else
			// allocate a new block
		{

			unsigned m = sizeof(union header) + n + 10*1024;
			ap->next = malloc(m);
			if(ap == NULL) {
				error("insufficient memory\n");
				exit(1);
			}
			ap->limit = (char *)ap + m;
		}
		// initialize the ap block
		 ap->avail = (char *)((union header *)ap+1);
		 ap->next = NULL;
		 // update arena[a] to point to the top of the stack
		 arena[a] = ap;
	}
	ap->avail += n;
	return ap->avail - n;
}

void deallocate(a) unsigned a; {
	arena[a]->next = freeblocks;
	freeblocks = first[a].next;
	first[a].next = NULL;
	arena[a] = &first[a];
}

void *newarray(m, n, a) unsigned long m, n; unsigned a; {
	return allocate(m*n,a);
}

#endif //PURIFY
