#include "parser.h"
#include "printer.h"
#include <stdio.h>
#include <stdlib.h>


int main() {

	T_Node child = T_constructChild(10, "node");
	T_Node node = T_constructContainer(10);

	T_addChild(node, child);
	T_addChild(node, child);


	printNode(node);

}
