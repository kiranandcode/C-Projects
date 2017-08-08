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

void printContainer(TS container) {

	printf("{\"type\":\"%d\",\"children\":[", container.type);

	int i;
	for(i = 0; i<container.count; i++) {

		printNode(container.children[i]);
		if(i != container.count-1)
			printf(",");
	}
	printf("]}");

}

void printNode(T node) {
	
	switch(node->type) {

		case CONTAINER:
			printContainer(node->Element.Container);
			break;

		case CHILD:
			printChild(node->Element.Child);
			break;

	}

}

