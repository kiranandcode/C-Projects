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
T T_constructChild(int type, char *value);
T T_constructContainer(int type);
void T_addChild(T container, T child);


#undef TS
#undef TC
#undef T

#endif  // PARSER_H
