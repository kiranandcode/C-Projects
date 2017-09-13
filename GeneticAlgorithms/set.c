#include "set.h"
#include "list.h"

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

	unsigned int (*hash)(void *);
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
	set->hash = NULL;
	assert(set->table);
	return set;
}

S set_newcustomhash(unsigned int table_size, unsigned int (*hash)(void *)) {
	assert(table_size);
	S set;
	set = malloc(sizeof(*set));
	set->table_size = table_size;
	set->length = 0;
	set->first = NULL;
	set->last = NULL;
	set->table = malloc(sizeof(*set->table)*table_size);
	set->hash = hash;
	assert(set->table);
	return set;
}

S set_copy(S set, void *(*copy)(void *)) {
	assert(set);
	S result;

	if(set->hash && set->hash != NULL) 
		result = set_newcustomhash(set->table_size, set->hash);
	else
		result = set_new(set->table_size);

	struct S_iterator iter = set_iterator(set);

	while(set_iteratorhasnext(&iter)) {
		void *item = set_iteratornext(&iter);
		if(copy && copy != NULL)
			item = copy(item);

		set_insert(result, item);
	}

	return result;
}

void set_delete(S set, void (*delete)(void *)) {
	assert(set);

	struct _set_S_elem *elem = set->first;
	struct _set_S_elem *next;

	while(elem != NULL) {
		if(delete && delete != NULL)
			delete(elem->elem);

		next = elem;
		elem = elem->next_l;
		free(next);
	}

	free(set->table);
	free(set);
}

void set_insert(S set, void *item)
{
	 unsigned int (*hash)(void *) = set->hash;
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


void set_remove(S set, void *item,
		void (*delete)(void *)) {
	assert(set);
	assert(set->table_size);
	assert(item);

        unsigned int (*hash)(void *) = set->hash;
	unsigned int position;
	if(hash && hash != NULL) {
		position = hash(item);	
	} else 
		position = ((unsigned int)item >> 3);

	position %= set->table_size;
	struct _set_S_elem **bucket,**prev_bucket;

	bucket = &(set->table[position]);
	prev_bucket = bucket;

	while(*bucket && *bucket != NULL) {
		// if already in the table return
		if((*bucket)->elem == item)
			break;
		prev_bucket = bucket;
		bucket = &(*bucket)->next_b;
	}

	if(*bucket == NULL)
		return;

	struct _set_S_elem *value = *bucket;

	*prev_bucket = (*bucket)->next_b;

	if((value)->prev_l == NULL) {
		assert(set->first == value);
		set->first = (value)->next_l;
	} else {
		assert((value)->prev_l->next_l == value);
		(value)->prev_l->next_l = (value)->next_l;
	}

	if((value)->next_l == NULL) {
		assert(set->last == value);
		set->last = (value)->prev_l;
	} else {
		assert((value)->next_l->prev_l == value);
		(value)->next_l->prev_l = (value)->prev_l;
	}



	if(delete && delete != NULL) {
		delete(value->elem);
	}
	
	free(value);
}

int set_contains(S set, void *item){
	assert(set);
	assert(set->table_size);
	assert(item);
	unsigned int (*hash)(void *) = set->hash;
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

S set_union(S setA, S setB, void *(*copy)(void *)){
	assert(setA);
	assert(setB);
	assert(setA->hash == setB->hash);

	S set = set_copy(setA, copy);
	struct S_iterator iter = set_iterator(setB);

	while(set_iteratorhasnext(&iter)) {
		void *item = set_iteratornext(&iter);
			if(copy && copy != NULL)
				item = copy(item);
			set_insert(set, item);
	}

	return set;
}

S set_intersect(S setA, S setB, void *(*copy)(void *)){
	assert(setA);
	assert(setB);
	assert(setA->hash == setB->hash);

	S set = set_new(setA->table_size);
	struct S_iterator iter = set_iterator(setA);

	while(set_iteratorhasnext(&iter)) {
		void *item = set_iteratornext(&iter);
		if(set_contains(setB, item)){
			if(copy && copy != NULL)
				item = copy(item);
			set_insert(set, item);
		}
	}

	return set;
}



S set_difference(S setA, S setB, void *(*copy)(void *)){
	assert(setA);
	assert(setB);
	assert(setA->hash == setB->hash);

	S set = set_new(setA->table_size);
	struct S_iterator iter = set_iterator(setA);

	while(set_iteratorhasnext(&iter)) {
		void *item = set_iteratornext(&iter);
		if(!set_contains(setB,item)) {
			if(copy && copy != NULL)
				item = copy(item);
			set_insert(set, item);
		}
	}
       	return set;
}


S set_symmetricdifference(S setA, S setB, void *(*copy)(void *)){
	assert(setA);
	assert(setB);
	assert(setA->hash == setB->hash);

	S set = set_new(setA->table_size);
	struct S_iterator iter = set_iterator(setA);

	while(set_iteratorhasnext(&iter)) {
		void *item = set_iteratornext(&iter);
		if(!set_contains(setB,item)) {
			if(copy && copy != NULL)
				item = copy(item);
			set_insert(set, item);
		}
	}
        iter = set_iterator(setB);

	while(set_iteratorhasnext(&iter)) {
		void *item = set_iteratornext(&iter);
		if(!set_contains(setA,item)) {
			if(copy && copy != NULL)
				item = copy(item);
			set_insert(set, item);
		}
	}

	return set;
}

int set_eq(S setA, S setB) {

	assert(setA);
	assert(setB);
	if(setA->hash != setB->hash) return 0;

	struct S_iterator iter = set_iterator(setA);

	while(set_iteratorhasnext(&iter)) {
		void *item = set_iteratornext(&iter);
		if(!set_contains(setB,item)) {
			return 0;
		}
	}
        iter = set_iterator(setB);

	while(set_iteratorhasnext(&iter)) {
		void *item = set_iteratornext(&iter);
		if(!set_contains(setA,item)) {
			return 0;
		}
	}

	return 1;
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

list_L set_tolist(S set, void *(*copy)(void *)) {
	assert(set);

	list_L list = list_new();
	struct S_iterator iter = set_iterator(set);

	while(set_iteratorhasnext(&iter)) {
		void *item = set_iteratornext(&iter);
		if(copy && copy != NULL)
			item = copy(item);
		list_push(list, item);
	}

	return list; 
}

S set_fromlist(list_L list, unsigned int table_size,
		unsigned int (*hash)(void *),
  		void *(*copy)(void *)){

	S set = set_newcustomhash(table_size, hash);

	struct L_iterator iter = list_iterator(list);

	while(list_iteratorhasnext(&iter)) {
		void *item = list_iteratornext(&iter);
		if(copy && copy != NULL) 
			item = copy(item);
		set_insert(set,item);
	}
	return set;
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
