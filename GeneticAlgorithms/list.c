#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define L list_L


struct L {
	size_t length;
	struct _L *elements;
};

struct _L {
	void *item;
	struct _L *next;
};

L list_new() {
	L list;
	list = malloc(sizeof(*list));
	list->length = 0;
	list->elements = NULL;
	return list;
}

L list_copy(L list, void *copy_elem(void *)) {
	L copy;
	copy = list_new();
	struct L_iterator iter = list_iterator(list);
	while(list_iteratorhasnext(&iter)){
		void *item = list_iteratornext(&iter);
		if(copy_elem && copy_elem != NULL) {
			item = copy_elem(item);
		}
		list_append(copy, item);
	}
	return copy;
}

void list_concat(L list, L second, void *(*copy_elem)(void *)) {
	struct L_iterator iter = list_iterator(second);
	while(list_iteratorhasnext(&iter)){
		void *item = list_iteratornext(&iter);
		if(copy_elem && copy_elem != NULL) {
			item = copy_elem(item);
		}
		list_append(list, item);
	}
	return;
}

L list_slice(L list, unsigned int start, unsigned int end,
		void *(*copy_elem)(void *)) {
	assert(list);
	assert(start < list->length && end <= list->length &&
		start < end);
	L slice = list_new();
	struct L_iterator iter = list_iterator(list);
	unsigned int current = 0;
	while(current < start && list_iteratorhasnext(&iter)) 
		list_iteratornext(&iter), current++;

	assert(current == start);
	list_append(slice, 
	(copy_elem && copy_elem != NULL) ? copy_elem(list_iteratorcurrent(&iter)) : list_iteratorcurrent(&iter));

	while(current < end && list_iteratorhasnext(&iter)){
		void *item = list_iteratornext(&iter);
		if(copy_elem && copy_elem != NULL) {
			item = copy_elem(item);
		}
		list_append(list, item);
		current++;
	}
	assert(current == end);
	
	return slice;
}

void list_delete(L list, void (*delete)(void *)) {
	assert(list);

	struct _L *i = list->elements;
	while(i != NULL) {
		if(delete && delete != NULL) {
			delete(i->item);
		}
		struct _L *to_del = i;
		i = i->next;
		free(to_del);
	}
}

void list_map(L list, void *(*f)(void *)){
	struct L_iterator iter = list_iterator(list);
	while(list_iteratorhasnext(&iter)){
		((struct _L*)iter.current)->item = f(((struct _L*)iter.current)->item);
		list_iteratornext(&iter);
	}
}

L list_filter(L list, int (*pred)(void *), void *(*copy_elem)(void *)) {
	L copy;
	copy = list_new();
	struct L_iterator iter = list_iterator(list);
	while(list_iteratorhasnext(&iter)){
		void *item = list_iteratornext(&iter);
		if(pred(item)){
			if(copy_elem && copy_elem != NULL) {
				item = copy_elem(item);
			}
			list_append(copy, item);
		}
	}
	return copy;
}

void *list_get(L list, unsigned int position) {
	assert(position < list->length);
	unsigned int i = 0;
	struct _L *current = list->elements;
	while(i < position && current && current != NULL) {
		++i;
		current = current->next;	
	}
	current = current && current != NULL ? current->item : NULL;
	return current;
}

void list_insert(L list, void *item, unsigned int position) {
	assert(list);
	assert(position <= list->length);

	++list->length;

	unsigned int i = 0;
	struct _L **current = &list->elements;

	struct _L *value = malloc(sizeof(*item));
	value->item = item;

	while(i < position && *current && *current != NULL){ 
		current = &((*current)->next);
		++i;
	}

	value->next = (*current);
	(*current) = value;

}

void *list_removeat(L list, unsigned int position) {
	assert(list);
	assert(position < list->length);
	--list->length;

	unsigned int i = 0;
	struct _L **current = &list->elements, **prev = current;

	while(i < position && *current && *current != NULL){ 
		prev = current;
		current = &((*current)->next);
		++i;
	}
	
	void *item = (*current)->item;
	if(prev != current) {

		struct _L *old =  (*current);
		(*prev)->next = (*current)->next;

		assert(old);

		free(old);

	}
	else {
		struct _L *old =  (*current)->next;
		free(*current);
		(*prev) = old;
	}
	return item;
}

void *list_remove(L list, void *item) {
	assert(list);
	--list->length;

	unsigned int i = 0;
	struct _L **current = &list->elements, **prev = current;

	while(*current && *current != NULL && (*current)->item != item){ 
		prev = current;
		current = &((*current)->next);
		++i;
	}
	
	if(*current && *current != NULL && (*current)->item == item) {

		void *item = (*current)->item;

		if(prev != current) {
			struct _L *old =  (*current);
			(*prev)->next = (*current)->next;
			free(old);
		}
		else {
			struct _L *old =  (*current)->next;
			free(*current);
			(*prev) = old;
		}
		return item;
	}
	return NULL;
}

void list_append(L list, void *item) {
	assert(list);

	++list->length;
	struct _L *current = list->elements;
	while(current != NULL && current->next != NULL) {
		current = current->next;
	}
	if(current && current != NULL) {
		(current->next) = malloc(sizeof(*current->next));
		current->next->next = NULL;
		current->next->item = item;
	} else {

		list->elements = malloc(sizeof(*list->elements));
		list->elements->next = NULL;
		list->elements->item = item;
	}
}

void list_print(L list, void (*print)(void *)) {
	assert(list);

	struct _L *i;
	for(i = list->elements; i != NULL; i = i->next) {
		if(print && print != NULL) {
			print(i->item);
		} else {
		       printf("%p", i->item);
		}
		printf(", ");
	}
}


struct L_iterator list_iterator(L list) {
	assert(list);
	struct L_iterator result;
	result.current = list->elements;
	return result;
}


int list_iteratorhasnext(struct L_iterator *list_iterator) {
	assert(list_iterator);
	return list_iterator->current != NULL;
}

void *list_iteratornext(struct L_iterator *list_iterator) {
	assert(list_iterator);
	assert(list_iterator->current);
	void *result = ((struct _L *)list_iterator->current)->item;
	list_iterator->current = ((struct _L *)list_iterator->current)->next;

	return result;
}

void *list_iteratorcurrent(struct L_iterator *list_iterator) {
	assert(list_iterator);
	assert(list_iterator->current);
	return  list_iterator->current;
}



unsigned int list_length(L list) {
	assert(list);
	return list->length;
}

void list_push(L list, void *item) {
	assert(list);
	list_insert(list, item, 0);
}
void *list_pop(L list) {
	assert(list);
	return list_removeat(list,0);
}


#undef L
