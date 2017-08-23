#ifndef C_H
#define C_H

//exported macros

// old compilers (pre-ANSI) do not want parameters in predeclarations
#ifdef __STDC__
#define ARGS(list) list
#else
#define ARGS(list) ()
#endif 

// old compilers have different variadic functions
//  also the header for varargs changes between C, ANSCI-c
#ifdef __STDC__
#include <stdarg.h>
#define va_init(a,b) va_start(a,b)
#else
#include <varargs.h>
#define va_init(a,b) va_start(a)
#endif 

// pre ansicompilers also require parameter lists rather than direct ones
#ifdef __STDC__
#define VARARGS(newlist,oldlist,olddcls) newlist
#else
#define VARARGS(newlist,oldlist,olddcls) oldlist olddcls
#endif

#define NULL ((void *)0)
#define NELEMS(a) ((int)(sizeof(a)/sizeof ((a)[0])))
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))

//typedefs

// interfaces

// exported types


// expreted functions

#endif //C_H
