#ifndef TYPES_H
#define TYPES_H
#include "c.h"
#include "sym.h"

typedef struct type *Type;
struct type {
	int op;
	Type type;
	int align;
	int size;
	union {
		// types with names or tags
		
		Symbol sym;
		// function types
	} u;
	Xtype x;
};

extern Type chartype;
extern Type doubletype;
extern Type floattype;
extern Type inttype;
extern Type longdouble;
extern Type longtype;
extern Type shorttype;
extern Type signedchar;
extern Type unsignedchar;
extern Type unsignedlong;
extern Type unsignedshort;
extern Type unsignedtype;
extern Type voidptype;
extern Type voidtype;

#endif //TYPES_H
