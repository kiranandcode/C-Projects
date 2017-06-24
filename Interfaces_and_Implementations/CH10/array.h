#ifndef ARRAY_H
#define ARRAY_H
// Array.h is a simple interface which hides most of the internal representation of the array object.
// this reduces dependancies, however arrayrep allows developers who need the internal structure to explicitly indicate their dependance.

#define T Array_T

typedef struct T *T;

extern T Array_new(int length, int size);
extern void Array_free(T *array);

extern int Array_length(T array);
extern int Array_size(T array);

extern void *Array_get(T array, int i);
// Array put returns a pointer to the element inserted, unlike Table_put which would return a pointer to the previous element
extern void *Array_put(T array, int i, void *elem);

extern void Array_resize(T array, int length);
extern T Array_copy(T array,, int length);



#undef T
#endif
