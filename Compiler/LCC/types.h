#ifndef TYPES_H
#define TYPES_H
#include "c.h"
#include "sym.h"

#define isqual(t) ((t)->op >= CONST)
#define unqual(t) (isqual(t) ? (t)->type :  (t))

#define isvolatile(t) ((t)->op == VOLATILE \
		|| (t)->op == CONST+VOLATILE)
#define isconst(t)   ((t)->op == CONST \
		|| (t)->op == CONST+VOLATILE)

#define isarray(t) (unqual(t)->op == ARRAY)
#define isstruct(t) (unqual(t)->op == STRUCT \
		|| unqual(t)->op == UNION)
#define isunion(t) (unqual(t)->op == UNION)
#define isfunc(t) (unqual(t)->op == FUNCTION)
#define isptr(t) (unqual(t)->op == POINTER)
#define ischar(t) (unqual(t)->op == CHAR)
#define isint(t) (unqual(t)->op >= CHAR \
		&& unqual(t)->op <= UNSIGNED)
#define isfloat(t) (unqual(t)->op <= DOUBLE)
#define isarith(t) (unqual(t)->op <= UNSIGNED)
#define isunsigned(t) (unqual(t)->op == UNSIGNED)
#define isdouble(t) (unqual(t)->op == DOUBLE)
#define isscalar(t) (unqual(t)->op <= POINTER \
		|| unqual(t)->op == ENUM)
#define isenum(t) (unqual(t)->op == ENUM)

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
