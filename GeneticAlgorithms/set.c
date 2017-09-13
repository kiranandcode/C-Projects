#include "set.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define S set_S

struct _set_S_elem {
	// pointers for iteration
	struct _set_S_elem *next_l, *prev_l;
	// pointers for bucket
	struct _set_S_elem *next_b;
	// element
	void *elem;
};


struct S {
	struct _set_S_elem **table;
	unsigned int table_size;
	unsigned int length;

	struct _set_S_elem *first;
	struct _set_S_elem *last;
};


S set_new(unsigned int table_size) {
	assert(table_size);
	S set;
	set = malloc(sizeof(*set));
	set->table_size = table_size;
	set->length = 0;
	set->first = NULL;
	set->last = NULL;
	set->table = malloc(sizeof(*set->table)*table_size);
	assert(set->table);
	return set;
}


void set_insert(S set, void *item, unsigned int (*hash)(void *)) {
	assert(set);
	assert(set->table_size);
	assert(item);

	unsigned int position;
	if(hash && hash != NULL) {
		position = hash(item);	
	} else 
		position = ((unsigned int)item >> 3);

	position %= set->table_size;
	struct _set_S_elem **bucket, **prev_bucket;

	bucket = &(set->table[position]);
	prev_bucket = bucket;

	while(*bucket && *bucket != NULL) {
		// if already in the table return
		if((*bucket)->elem == item)
			return;
		prev_bucket = bucket;
		bucket = &(*bucket)->next_b;
	}
	struct _set_S_elem * new_o;
	new_o = malloc(sizeof(*new_o));
	assert(new_o);
	new_o->elem = item;

	new_o->next_b = NULL;
	new_o->next_l = NULL;
	new_o->prev_l = set->last;

	if(set->first == NULL){
		assert(set->last == NULL);
		set->first = new_o;
		set->last = new_o;
	} else {
		assert(set->last != NULL);
		set->last->next_l = new_o;
		set->last = new_o;
	}


	if(*prev_bucket == NULL) {
		assert(prev_bucket == bucket);
		*prev_bucket = new_o;
	} else {
		assert(*bucket == NULL);
		(*prev_bucket)->next_b = new_o;
	}
	set->length++;
}

int set_contains(S set, void *item, unsigned int (*hash)(void *)){
	assert(set);
	assert(set->table_size);
	assert(item);

	unsigned int position;
	if(hash && hash != NULL) {
		position = hash(item);	
	} else 
		position = ((unsigned int)item >> 3);

	position %= set->table_size;
	struct _set_S_elem **bucket;

	bucket = &(set->table[position]);

	while(*bucket && *bucket != NULL) {
		// if already in the table return
		if((*bucket)->elem == item)
			return 1;
		bucket = &(*bucket)->next_b;
	}
	return 0;
}


void set_print(S set, void (*print)(void *)) {
	assert(set);

	struct S_iterator iter = set_iterator(set);

	printf("{");
	while(set_iteratorhasnext(&iter)) {
		void *item = set_iteratornext(&iter);

		if(print && print != NULL) {
			print(item);
		} else {
			printf("%p", item);
		}
		printf(", ");
	}
	printf("}");
}


struct S_iterator set_iterator(S set) {
	assert(set);

	struct S_iterator result;
	result.current = set->first;
	return result;
}

int set_iteratorhasnext(struct S_iterator *curr) {
	return curr->current != NULL;
}

void *set_iteratornext(struct S_iterator *curr) {
	assert(curr->current != NULL);
	void *result = ((struct _set_S_elem *)curr->current)->elem;

	curr->current = ((struct _set_S_elem *)curr->current)->next_l;
	return result;
}

#undef S
