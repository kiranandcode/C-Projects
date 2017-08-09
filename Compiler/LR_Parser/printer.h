#ifndef PRINTER_H
#define PRINTER_H

#include "parser.h"

#define TS T_Container
#define TC T_Child
#define T  T_Node


void printChild(TC child);
void printContainer(TS container);
void printNode(T node);

#undef TS
#undef TC
#undef T


#endif
