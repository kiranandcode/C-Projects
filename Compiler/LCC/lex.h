#ifndef LEX_H
#define LEX_H

#include "sym.h"

extern int getchr ARGS((void));
extern int gettok ARGS((void));


extern int t;
extern char *token;
extern Symbol tsym;
extern Coordinate src;


#endif
