#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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
#endif
