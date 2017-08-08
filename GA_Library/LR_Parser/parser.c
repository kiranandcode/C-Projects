#include "parser.h"

#include "parser_rep.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// development structure
// --------------------
// Goal: Build a LR parser
//
//  - configure general structure for AST
//  - set up parser and goto tables
//  - set up driver for parser tables
//  - test driver






// General Structure for AST.
// -------------------------
// Goal: Build a general structure for later use.
// Examples: taking the AST and converting into string
// 
// struct container - contains a set of elements,
// struct child     - contains a single value


#define TS T_Container
#define TC T_Child
#define T  T_Node
#define NEW(x) ((x) = malloc((long)sizeof(*(x))))

T constructChild(int type, char *value) {

	T child;
	NEW(child);

	child->type = CHILD;

	child->Element.Child.type = type;
	child->Element.Child.value = malloc(sizeof(char)*strlen(value)+1);
	strcpy(child->Element.Child.value, value);

	return child;
}


