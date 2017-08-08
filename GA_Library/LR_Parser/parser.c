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

T T_constructChild(int type, char *value) {

	T child;
	NEW(child);

	child->type = CHILD;

	child->Element.Child.type = type;
	child->Element.Child.value = malloc(sizeof(char)*strlen(value)+1);
	strcpy(child->Element.Child.value, value);

	return child;
}

T T_constructContainer(int type) {
	T container;
	NEW(container);

	container->type = CONTAINER;

	container->Element.Container.type = 0;
	container->Element.Container.count = 0;
	container->Element.Container.children = NULL;

	return container;

}


void T_addChild(T container, T child) {


	switch(container->type) {

		case CONTAINER: {
			int len = container->Element.Container.count++;
			container->Element.Container.children = realloc(container->Element.Container.children, container->Element.Container.count * (sizeof(*container)));
			container->Element.Container.children[len] = child;

			break;

				}
		case CHILD:{

				   T new_child = T_constructChild(container->Element.Child.type, container->Element.Child.value);
				   free(container->Element.Child.value);
				   container->type = CONTAINER;
				   container->Element.Container.type = container->Element.Child.type;
				   container->Element.Container.count = 0;
				   container->Element.Container.children = NULL;
				   T_addChild(container, new_child);
				   T_addChild(container, child);

			break;
			   }

	}

}

