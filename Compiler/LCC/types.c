#include "types.h"
#include "string.h"

#include <stdlib.h>


// ------------------------------------------
// Types 
// ------------------------------------------


// ------------------------------------------
//  Data 
// ------------------------------------------

static struct entry {
	struct type type;
	struct entry *link;
} *typetable[128];


static int maxlevel;



// ------------------------------------------
// Function 
// ------------------------------------------
static Type type(op, ty, size, align, sym)
	int op, size, align; Type ty; void *sym; {

		unsigned h = /* hash op and ty */
		       (op ^ ((unsigned)ty>>3))	&(NELEMS(typetable)-1);
		struct entry *tn;
		if(op != FUNCTION && (op != ARRAY || size > 0)
				// search for existing type
	for(tn = typetable[h]; tn; tn = tn->link)
	if(tn->type.op == op && tn->type.type == ty
	&& tn->type.size == size && tn->type.align == align
	&& tn->type.u.sym == sym)
	return &tn->type; 
		NEW(tn, PERM);
		tn->type.op = op;
		tn->type.type = ty;
		tn->type.size = size;
		tn->type.align = align;
		tn->type.u.sym = sym;
		memset(&tn->type.x, 0, sizeof tn->type.x);
		tn->link = typetable[h];
		typetable[h] = tn;
		return &tn->type;

	}

void typeInit() {
#define xx(v,name,op,metrics) { \
	Symbol p = install(string(name), &types, GLOBAL, PERM);\
	v = type(op, 0, IR->metrics.size, IR->metrics.align, p);\
	p->type = v; p->addressed = IR->metrics.outofline; }
	xx(chartype,      "char",           CHAR, charmetric);
	xx(doubletype,    "double",         DOUBLE, doublemetric);
	xx(floattype,     "float",          FLOAT, floatmetric);
	xx(inttype,       "int",            INT, intmetric);
	xx(longdouble,    "long double",    DOUBLE, doublemetric);
	xx(longtype,      "long int",       INT, intmetric);
	xx(shorttype,     "short",          SHORT, shortmetric);
	xx(signedchar,    "signed char",    CHAR, charmetric);
	xx(unsignedchar,  "unsigned char",  CHAR, charmetric);
	xx(unsignedlong,  "unsignedlong",   UNSIGNED, intmetric);
	xx(unsignedshort, "unsigned short", SHORT, shortmetric);
	xx(unsignedtype,  "unsigned int",   UNSIGNED, intmetric);
	{
		Symbol p;
		p = install(string("void"), &types, GLOBAL, PERM);
		voidtype = type(VOID, NULL, 0, 0, p);
		p->type = voidtype;
	}

}

void rmtypes(lev) int lev; {
	
	if(maxlevel >= lev) {
		int i;
		maxlevel = 0;
		for(i = 0; i < NELEMS(typetable); ++i) {
			// remove types with u.sym->scope >= lev
	struct entry *tn, **tq = &typetable[i];
	while((tn = *tq) != NULL)
		// if a function remove type
		if(tn->type.op  == FUNCTION)
			tq = &tn->link;
	else if(tn->type.u.sym && tn->type.u.sym->scope >= lev)
		*tq = tn->link;
	else {
		// recompute maxlevel
		if(tn->type.u.sym && tn->type.u.sym->scope > maxlevel)
			maxlevel = tn->type.u.sym->scope;
		tq = &tn->link;
	}
		}
	}
}


Type ptr(typ) Type ty; {
	return type(POINTER, ty, IR->ptrmetric.size,
			IR->ptrmetric.align, pointersym);
}
