#include "sym.h"
#include "alloc.h"
#include "string.h"
#include "types.h"

#include <stdlib.h>
#include <string.h>


// ------------------------------------------------------
// data 
// ------------------------------------------------------
struct symbol {
	char *name;
	int scope; // classified into constants, labels, global, param, local
	Coordinate src; // location of symbol definition
	Symbol up; // <-- chains all symbols in table, from the last installed
	          // by traversing the chain all symbols currently in scope can be found
	List uses; // contains a  list of all coordinates a symbol occurs
	int sclass; // storage class
	//             one of AUTO, REGISTER, STATIC, EXTERN, TY..
	// symbol flags (50)
	unsigned temporary:1;
	unsigned generated:1;
	unsigned defined:1;

	Type type;
	float ref;
	union {

		// labels
		struct {
			int label;
			Symbol equatedto;
		} l;

	
		// types
		
		// enum constants
		

		// enum types
		
		// constants
		struct {
			Value v;
			Symbol loc;
		} c;

		// function symbols

		// globals
		
		// temporaries

	} u; // additional info
	Xsymbol x; // backend stuff
	//debugger extension
};


// a table is a structure that holds a hash table for symbols at
// a given scope. the scope is given by the value of the level 
// field.
// essentially each exported table value is like a stack.
// looking up an entry involves navigating down the linked list
// of it's values, and then if not found, repeating the procedure
// on the previous table
struct table {
	int level; // the scope level
	Table previous; // the enclosing scopes table
	struct entry { // hashtable for all symbols at scope
		struct symbol sym;
		struct entry *link;
	} *buckets[256];
	Symbol all; // a threaded list of all symbols so far
};



static struct table
	cns = { CONSTANTS, NULL, {}, NULL },
	ext = { GLOBAL, NULL, {}, NULL },
	ids = { GLOBAL, NULL, {}, NULL },
	tys = { GLOBAL, NULL, {}, NULL };

int level = GLOBAL;
Table constants = &cns;
Table externals = &ext;
// points to the current scope
Table identifiers = &ids;
// always points to the global scope table
Table globals = &ids;
Table types = &tys;
Table labels;


// ------------------------------------------------------
// function 
// ------------------------------------------------------
Table table(tp, level) Table tp; int level; {
	Table new;
	NEWO(new,FUNC);
	new->previous = tp;
	new->level = level;
	if(tp)
		new->all = tp->all;
	return new;
}

void foreach(tp,lev,apply,cl) Table tp; int lev;
	void (*apply) ARGS((Symbol, void *)); void *cl; {
	while(tp && tp->level > lev)
		tp = tp->previous;
	if(tp && tp->level == lev) {
		Symbol p;
		Coordinate sav;
		sav = src;
		for(p = tp->all; p && p->scope == lev; p = p->up) {
			src = p->src;
			(*apply)(p,cl);
		}
		src = sav;

		}
}

void enterscope() {
	++level;
}


void exitscope() {
	rmtypes(level);

	if(types->level == level)
		types = types->previous;
	if(identifiers->level == level) {
		// warn if more than 127 identifiers
		identifiers = identifiers->previous;
	}
	--level;
}


// given a character, installs it into a table
Symbol install(name, tpp, level, arena)
	char *name; Table *tpp; int level, arena; {
	Table tp = *tpp;
	struct entry *p;
	// name is a stored unique string, so the pointer is the hash
	unsigned h = (unsigned)name&(HASHSIZE-1);

	// create a new table if necassary
	if(level > 0 && tp->level < level) 
		tp = *tpp = table(tp,level);
	NEWO(p,arena);
	p->sym.name = name;
	p->sym.scope = level;
	p->sym.up = tp->all;
	tp->all = &p->sym;
	// each bucket is a stack
	p->link = tp->buckets[h];
	tp->buckets[h] = p;
	return &p->sym;
}


Symbol lookup(name, tp) char * name; Table tp; {
	struct entry *p;
	// use the pointer as the hash
	unsigned h = (unsigned)name&(HASHSIZE-1);

	// keep on descending the hash chain
	do
		for(p = tp->buckets[h]; p; p = p->link)
			if(name == p->sym.name)
				return &p->sym;
	// traverse up scope
	while ((tp = tp->previous) != NULL);
	return NULL;
}

int genlabel(n) int n; {
	static int label = 1;
	label += n;
	return label -n;
}

Symbol findlabel(lab) int lab; {

	struct entry *p;
	unsigned h = lab & (HASHSIZE-1);

	// loop through the chain of labels to find the 
	// symbol with a matching label
	for(p = labels->buckets[h]; p; p = p->link)
		if(lab == p->sym.u.l.label)
			return &p->sym;

	// if not found insert it
	NEWO(p, FUNC);
	// name is a stringified version of the label
	p->sym.name = stringd(lab);
	p->sym.scope = LABELS;
	p->sym.up = labels->all;
	labels->all = &p->sym;
	// each bucket in labels is like a hash
	p->link = labels->buckets[h];
	labels->buckets[h] = p;
	p->sym.generated = 1;
	p->sym.u.l.label = lab;
	(*IR->defsymbol)(&p->sym);
	return &p->sym;
}


Symbol constant(ty, v) Type ty; Value v; {
	struct entry *p;
	unsigned h = v.u&(HASHSIZE-1);

	ty= unqual(ty);
	// loop down the hashed chain
	for(p = constants->buckets[h]; p; p = p->link)
		if(eqtype(ty, p->sym.type, 1))
			// return the symbol if p's value == v
			switch(ty->op) {
				case CHAR:
					if(equalp(uc)) return &p->sym; break;
				case SHORT:
					if(equalp(ss)) return &p->sym; break;
				case INT:
					if(equalp(i)) return &p->sym; break;
				case UNSIGNED:
					if(equalp(u)) return &p->sym; break;
				case FLOAT:
					if(equalp(f)) return &p->sym; break;
				case DOUBLE:
					if(equalp(d)) return &p->sym; break;
				case ARRAY: case FUNCTION:
				case POINTER:
					if(equalp(p)) return &p->sym; break;
			}
	// if the constant is not found, create it
	NEWO(p,PERM);
	p->sym.name = vtoa(ty,v);
	p->sym.scope = CONSTANTS;
	p->sym.type = ty;
	p->sym.sclass = STATIC;
	p->sym.u.c.v = v;
	p->link = constants->buckets[h];
	p->sym.up  = constants->all;
	constants->all = &p->sym;

	// announce the constant
	// for debug purposes
	if(ty->u.sym && !ty->u.sym->addressed)
		(*IR->defsymbol)(&p->sym);
	

	p->sym.defined = 1;
	return &p->sym;
}


Symbol intconst(n) int n; {
	Value v;
	v.i = n;
	return constant(inttype, v);
}

Symbol genident(scls, ty, lev) int scls,lev; Type ty; {
	Symbol p;

	NEWO(p, lev >= LOCAL ? FUNC : PERM);

	p->name = stringd(genlabel(1));
	p->scope = lev;
	p->sclass = scls;
	p->type = ty;
	p->generated = 1;
	if(lev == GLOBAL)
		(*IR->defsymbol)(p);
	return p;
}

Symbol temporary(scls, ty, lev) Type ty; int scls, lev; {
	Symbol p = genident(scls, ty, lev);

	p->temporary = 1;
	return p;
}

Symbol newtemp(sclass, tc) int sclass, tc; {
	Symbol p = temporary(sclass, btot(tc), LOCAL);

	(*IR->local)(p);
	p->defined = 1;
	return p;
}


