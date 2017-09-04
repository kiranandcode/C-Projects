#include "tree.h"


Tree tree(op, type, left, right) 
	int op; Type type; Tree left, right; {
	Tree p;

	NEWO(p, where);
	p->op = op;
	p->type = type;
	p->kids[0] = left;
	p->kids[1] = right;
	return p;
	}


Tree texpr(f, tok, a) Tree (*f) ARGS((int)); int tok, a; {
	int save = where;
	Tree p;
	where = a;
	p = (*f)(tok);
	where = save;
	return p;
}


Tree expr(tok) int tok; {
	static char stop[] = {IF,ID, '}', 0};
	Tree p = expr1(0);
	while(t == ',') {
		tree q;
		t = gettok();
		q = pointer(expr1(0));
		p = tree(RIGHT, q->type, root(value(p)),q);
	}
	// terst for correct termination
	return p;
}
