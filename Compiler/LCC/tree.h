#ifndef TREE_H
#define TREE_H
#include "c.h"

typedef struct tree *Tree;
struct tree {
	int op; // for the operator
	Type type; // type of the result
	Tree kids[2]; //  children
	Node node;
	union {
		// fields for tree variants
	} u;
};

#endif //TREE_H
