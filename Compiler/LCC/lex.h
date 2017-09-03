#ifndef LEX_H
#define LEX_H

#include "sym.h"

#define MAXTOKEN 32
#define istypename(t,tsym) (kind[t] == CHAR \
		|| t == ID && tsym && tsym->sclass == TYPEDEF)

extern int getchr ARGS((void));
extern int gettok ARGS((void));


extern int t;
extern char *token;
extern Symbol tsym;
extern Coordinate src;


#endif
