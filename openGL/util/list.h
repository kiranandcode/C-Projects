#ifndef LIST_H
#define LIST_H

#define L list_L

struct L;
typedef struct L *L;

struct L_iterator {
	void *current;
};

L list_new();
L list_copy(L list, void *(*copy)(void *));
void list_concat(L list, L second, void *(*copy)(void *));
L list_slice(L list, unsigned int start, unsigned int end,
		void *(*copy)(void *));
void list_delete(L list, void (*delete)(void *));
void list_map(L list, void *(*f)(void *));
L list_filter(L list, int (*pred)(void *), void *(*copy_elem)(void *));
void *list_get(L list, unsigned int position);
void list_insert(L list, void *item, unsigned int position);
void *list_removeat(L list, unsigned int position);
void *list_remove(L list, void *item);
void list_append(L list, void *item);
void list_print(L list, void (*print)(void *));
struct L_iterator list_iterator(L list);
int list_iteratorhasnext(struct L_iterator *list_iterator);
void *list_iteratornext(struct L_iterator *list_iterator);
void *list_iteratorcurrent(struct L_iterator *list_iterator);
unsigned int list_length(L list);
void list_push(L list, void *item);
void *list_pop(L list);

#undef L
#endif
