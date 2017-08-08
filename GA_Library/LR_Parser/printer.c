#include "printer.h"

#include "parser.h"
#include "parser_rep.h"

#include <stdio.h>


#define TS T_Container
#define TC T_Child
#define T  T_Node

void printChild(TC child) {
	printf("{\"type\":\"%d\",\"value\":\"%s\"}", child.type, child.value);

}


void printNode(T node) {
	
	switch(node->type) {

		case CONTAINER:
			break;

		case CHILD:
			printChild(node->Element.Child);
			break;

	}

}
