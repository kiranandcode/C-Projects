#include "table.h"
#include "list.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define T table_T

struct T {
	int size;
	int timestamp;
	int length;
	int (*cmp)(const void *x, const void *y);
	unsigned (*hash) (const void *key);

	struct binding *first;
	struct binding *last;

	struct binding {
		struct binding *next;
		struct binding *prev;
		struct binding *link;
		const void *key;
		void *value;
	} **buckets;
};

static int cmpatom(const void *x, const void *y) {
	return x != y;
}

static unsigned hashatom(const void *key) {
	// sometimes the first couple of bits are zeroed out
	return (unsigned long) key >> 2;
}



T table_new(int hint, int cmp(const void *x, const void *y),
		unsigned hash(const void *key)) {
	static int primes[] = {
		13, 23, 47, 103, 237, 509, 509, 1021, 2053, 4093, 8191, 16381, 65521, INT_MAX
	};
	assert(hint >= 0);
	int i;
	for(i = 1; primes[i] < hint; ++i)
		;

	T table;
	table = malloc(sizeof(*table) + primes[i-1] * sizeof(table->buckets[0]));
	assert(table != NULL);

	table->size = primes[i-1];
	table->length = 0;
	table->timestamp = 0;
	table->cmp = cmp != NULL ? cmp :  cmpatom;
	table->hash = hash != NULL ? hash : hashatom;

	table->first = NULL;

	table->buckets = (struct binding **)(table + 1);
	for(i = 0; i < table->size; ++i) 
		table->buckets[i] = NULL;

	return table;
}

void Table_free(T *table, void delete(void *)) {
	assert(table && *table);
	if((*table)->length > 0) {
		struct binding *p, *q = NULL;
		for(p = (*table)->first; p && p != NULL; p = q) {
			if(delete && delete != NULL)
				delete(p->value);

			q = p->next;
			free(p);
		}
	}
	free(*table);
	*table = NULL;
}

int table_length(T table) {
	assert(table);
	return table->length;
}


void *table_put(T table, const void *key, void *value) {
	int i;
	struct binding *p = NULL;
	void *prev = NULL;

	assert(table);
	assert(key);

	i = (*table->hash)(key) % table->size;
	for(p = table->buckets[i]; p && p != NULL; p = p->link)
		if((*table->cmp)(key, p->key) == 0)
			break;
	if(p == NULL) {
		p = malloc(sizeof(*p));
		p->key = key;
		p->link = table->buckets[i];
		table->buckets[i] = p;
		table->length++;
		if(table->last == NULL) {
			assert(table->first == NULL);

			table->first = p;
			table->last = p;
			p->prev = NULL;
			p->next = NULL;
		} else {
			assert(table->first != NULL);
			assert(table->last->next == NULL);
			p->prev = table->last;
			p->next = NULL;
			table->last->next = p;
			table->last = p;
		}

		prev = NULL;
	} else
		prev = p->value;
	p->value = value;
	table->timestamp++;
	return prev;
}

void *table_get(T table, const void *key) {
	assert(table);
	assert(key);
	
	int i;
	struct binding *p = NULL;


	i = (*table->hash)(key) % table->size;
	for(p = table->buckets[i]; p && p != NULL; p = p->link){ 
		if((*table->cmp)(key, p->key) == 0)
			break;
	}
	return p;
}


void *table_remove(T table, const void *key) {
	int i;
	struct binding **pp;

	assert(table);
	assert(key);
	table->timestamp++;
	i = (*table->hash)(key) % table->size;
	for(pp = &table->buckets[i]; *pp && *pp != NULL; pp = &(*pp)->link) if((*table->cmp)(key, (*pp)->key) == 0) {
		struct binding *p = *pp;
		void *value = p->value;
		*pp = p->link;
		if(p->prev == NULL) {
			assert(table->first == p);
			table->first = p->next;
		} else {
			p->prev->next = p->next;
		}
		if(p->next == NULL) {
			assert(table->last == p);
			table->last = p->prev;
		} else {
			p->next->prev = p->prev;
		}

		free(p);
		table->length--;
		return value;
	}
	return NULL;
}





void table_map(T table, void apply(const void *key, void **value, void *cl), void *cl) {
	assert(table);
	assert(apply);
	unsigned stamp;
	struct binding *p;

	stamp = table->timestamp;
	p = table->first;

	for(; p && p != NULL; p = p->next) {
		apply(p->key, &p->value, cl);
		assert(table->timestamp == stamp);
	}
}


void table_print(T table, void printKey(const void *key), void printValue(void *value)) {
	assert(table);
	struct T_iterator iter = table_iterator(table);
	

	printf("{");
	while(table_iteratorhasnext(&iter)) {
		const void *key = table_iteratorgetkey(&iter);
		void *value = table_iteratornext(&iter);
		if(printKey && printKey != NULL)
			printKey(key);
		else 
			printf("%p", key);
		printf(": ");

		if(printValue && printValue != NULL) 
			printValue(value);
		else 
			printf("%p", value);
		printf(", ");
	}
	printf("}\n");
}


list_L table_toList(T table) {
	assert(table);

	list_L list = list_new();
	struct T_iterator iter = table_iterator(table);
	while(table_iteratorhasnext(&iter)) {
		void *result = table_iteratornext(&iter);
		list_append(list, result);
	}
	return list;
}

void table_fromList(T table, list_L keys, list_L values){ 
	assert(keys);
	assert(values);
	assert(table);
	struct L_iterator keyiter = list_iterator(keys);
	struct L_iterator valueiter = list_iterator(values);
	while(list_iteratorhasnext(&keyiter) && list_iteratorhasnext(&valueiter)) {
		const void *key = list_iteratornext(&keyiter);
		void *value = list_iteratornext(&valueiter);
		table_put(table, key, value);
	}
}



struct T_iterator table_iterator(T table) {
	assert(table);
	struct T_iterator iter;
	iter.val = table->first;
	return iter;
}

const void *table_iteratorgetkey(struct T_iterator *iterator){ 
	assert(iterator);
	assert(iterator->val);
	struct binding *p = iterator->val;
	return p->key;
}

void *table_iteratornext(struct T_iterator *iterator) {
	assert(iterator);
	assert(iterator->val);


	struct binding *p = iterator->val;
	void *res = p->value;
	iterator->val = p->next;

	return res;
}

int table_iteratorhasnext(struct T_iterator *iterator) {
	assert(iterator);
	return iterator->val && iterator->val != NULL;
}
