#ifndef PARSER_REP_H
#define PARSER_REP_H

#define TS T_Container
#define TC T_Child
#define T  T_Node

struct TS {
	int type;
	int count;
	struct T *children;
};

struct TC {
	int type;
	char *value;
};

enum Node_Type {
	CONTAINER, CHILD
};

struct T {
	enum Node_Type type;
	union {
		struct TS Container;
		struct TC Child;
	} Element;
};



#undef TS
#undef TC
#undef T

#endif //PARSER_REP_H
