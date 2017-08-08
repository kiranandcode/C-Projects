#ifndef PARSER_H
#define PARSER_H


#define TS T_Container
#define TC T_Child
#define T  T_Node

typedef struct TS TS;
typedef struct TC TC;
typedef struct T  *T;


/**
 *
 * Construct child
 * @param type The type of the child
 * @param value The actual value of the child
 *
 * */
T constructChild(int type, char *value);


#undef TS
#undef TC
#undef T

#endif  // PARSER_H
