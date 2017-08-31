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
static Symbol pointersym;




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

	pointersym = install(string("T*"), &types, GLOBAL, PERM);
	pointersym->addressed = IR->ptrmetric.outofline;
	voidptype = ptr(voidtype);

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


Type deref(ty) Type ty; {

	if(isptr(ty))
		ty = ty->type;
	else
		error("type error: %s\n", "pointer expected");
	return isenum(ty) ? unqual(ty)->type : ty;
}

Type array(ty, n, a) Type ty; int n,a; {
// c does not permit arrays of functions
	if(isfunc(ty)) {
		error("illegal type 'array of %t'\n", ty);
		return array(inttype, n, 0);
	}
// does not allow incomplete arrays at any scope other than global
	if(level > GLOBAL && isarray(ty) && ty->size == 0)
		error("missing array size\n");	
	if(ty->size == 0) {
		if(unqual(ty) == voidtype)
			error("illegal type 'array of %t'\n", ty);
		else if(Aflag >= 2)
			warning("declaring type 'array of %t' is undefined\n", ty);
		
	} else if(n > INT_MAX/ty->size) {
// c does not permit arrays of size greater than intmax
		error("size of 'array of %t' exceeds %d bytes\n",
				ty, INT_MAX);
		n = 1;
	}
	return type(ARRAY, ty, n*ty->size, a ? a : ty->align, NULL);

}

Type atop(ty) Type ty; {
	if(isarray(ty))
		return ptr(ty->type);
	error("type error: %s\n", "array expected");
	return ptr(ty);
}

Type qual(op, ty) int op; Type ty; {

	if(isarray(ty))
		ty - type(ARRAY, qual(op, ty->type), ty->size, ty->align, NULL);
	else if(isfunc(ty))
		warning("qualified function type ignored\n");
	else if(isconst(ty) && op == CONST
			|| isvolatile(ty) && op == VOLATILE)
		error("illegal type '%k %t'\n", op, ty);
	else {
		if(isqual(ty)) {
			op += ty->op;
			ty = ty->type;
		}
		ty = type(op, ty, ty->size, ty->align, NULL);
	}
	return ty;
}

Type func(ty, proto, style) Type ty, *proto; int style; {
	if(ty && (isarray(ty) || isfunc(ty)))
		error("illegal return type '%t'\n", ty);
	ty = type(FUNCTION, ty, 0, 0, NULL);
	ty->u.f.proto = proto;
	ty->u.f.oldstyle = style;
	return ty;
}

Type freturn(ty) Type ty; {
	if(isfunc(ty))
		return ty->type;
	error("type error: %s\n", "function expected");
	return inttype;
}

int variadic(ty) Type ty; {
	// returns whether a function is variadic
	if(isfunc(ty) && ty->u.f.proto) {
		int i;
		for(i = 0; ty->u.f.proto[i]; i++)
			;
		// variadic functions always have 1 declared variable + 1
		return i > 1 && ty->u.f.proto[i-1] == voidtype;
	}
	return 0;
}

Type newstruct(op, tag) int op; char *tag; {
	Symbol p;
	if(*tag == 0)
		tag = stringd(genlabel(1));
	else
		// check for redefinition of tag
		if((p = lookup(tag, types)) != NULL && (p->scope == level || p->scope == PARAM && level == PARAM+1)){
		if(p->type->op == op && !p->defined)
			return p->type;
		error("redefinition of '%s' previously defined at %w\n", p->name, &p->src);

		}
	p = install(tag, &types, level, PERM);
	p->type = type(op, NULL, 0,0,p);
	if(p->scope > maxlevel)
		maxlevel = p->scope;
	p->src = src;
	return p->type;
}


Field newfield(name, ty, fty) char *name; Type ty, fty; {
	Field p, *q = &ty->u.sym->u.s.flist;
	if(name == NULL)
		name = stringd(genlabel(1));
	for(p = *q; p; q = &p->link, p = *q)
		if(p->name == name)
			error("duplicate field name '%s' in '%t'\n", name, ty);
	NEWO(p,perm);
	*q = p;
	p->name = name;
	p->type = fty;
	return p;
}
