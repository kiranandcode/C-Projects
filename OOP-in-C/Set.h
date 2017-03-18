#ifndef SET_H
#define SET_H

// To add an element to the set
void* add(void* set, const void* element);

// To find an element in the set
void* find(const void* set, const void* element);

// To Remove an element in a set
void* drop(void* set, const void* element);

// To check whether a set contains an element.
int contains(const void* set, const void* element);

#endif // SET_H
