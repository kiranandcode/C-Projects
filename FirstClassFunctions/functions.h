#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "table.h"

// function -> accepts two params:
//             params     |
//             closure    |-----> list of (typename, size, value)
// 
// a function object consists of
//              {
//                     void *func(void *closure, void *params)
//                     void *closure;
//              }
//
// function application to params consists of 
//             applying func(closure, params) and returning the result
//                  


// functionB dot functionA(closure) -> new functionC: // which passes inputs to function a, then passes the result to functionB

struct F_args {
	table_T mapping;
};


struct function_F {
	struct F_args (*lambda)(struct F_args closure, struct F_args params);
	struct F_args closure;
};




#define F function_F
struct F;
typedef struct F F;
struct F_args;

#define get(args, name, type) (name) = *(type *)function_argsget((args), #name, #type)
#define var(type, name, value) #name, #type, sizeof(type), &value 


F function_new(struct F_args (*lambda)(struct F_args closure, struct F_args params), struct F_args closure);
void function_delete(F function);
F function_compose(F functionA, F functionB);
struct F_args function_apply(F function, struct F_args params);

struct F_args function_args(int count, ...);
void *function_argsget(struct F_args args, char *name, char *type);
void function_argsdelete(struct F_args *args);
#undef F
#endif
