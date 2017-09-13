#ifndef SET_H
#define SET_H
#define S set_S

#include "list.h"

struct S;
typedef struct S *S;

struct S_iterator {
	void *current;
};

S set_new(unsigned int table_size);
S set_newcustomhash(unsigned int table_size, unsigned int (*hash)(void *));
S set_copy(S set, void *(*copy)(void *));
void set_delete(S set, void (*delete)(void *));
void set_insert(S set, void *item);
void set_remove(S set, void *item,
		void (*delete) (void *));
int set_contains(S set, void *item);
S set_union(S setA, S setB, void *(*copy)(void *));
S set_intersect(S setA, S setB, void *(*copy)(void *));
S set_difference(S setA, S setB, void *(*copy)(void *));
S set_symmetricdifference(S setA, S setB, void *(*copy)(void *));
int set_eq(S setA, S setB);

void set_print(S set, void (*print)(void *));
list_L set_tolist(S set, void *(*copy)(void *));

struct S_iterator set_iterator(S set);
int set_iteratorhasnext(struct S_iterator *iter);
void *set_iteratornext(struct S_iterator *iter);
#undef S
#endif //SET_H
