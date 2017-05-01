
#ifndef GRAPHUTILS_LIST_H
#define GRAPHUTILS_LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef enum types {
    INT,
    LIST,
    COUNT
} types;


typedef struct listelem {
    struct list *ls;
    void *element;
    struct listelem *next;
} elem;


/**
 * In this library, lists are homogenous data structures - all elements of the same type. The list struct encapsulates the type of the list, the elements simply hold references.
 */
typedef struct list {
    elem *head;
    types type;
} list;


/**
 * Constructs a new list of the specified type.
 * @warning Types are not checked by the library, type checking must be implemented by user.
 * @param t The type param to use (see enum types)
 * @return new list object
 */
list *getNewList(types t);

void printList(list *ls);

void _print_INT(void * element);

void _print_LIST(void *element);

extern void (*print[COUNT]) (void *);

void append(list *ls, void *obj);


/**
 * A generic length function to return the length of an object of the specified type.
 * @param obj A pointer to an object.
 * @param t The type of the object.
 * @return integer representation of the length of the object.
 */
int length(void *obj, enum types t);

void *get(int index, list *ls);

void del(int index, list *ls);

void *pop(list *ls);

void printList(list *ls);







#endif //GRAPHUTILS_LIST_H
