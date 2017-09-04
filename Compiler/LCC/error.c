#include "error.h"
#include "token.h"

#include <stdlib.h>

int errcnt = 0;
int errlimit = 20;


char kind[] = {
#define xx(a,b,c,d,e,f,g) f,
#define yy(a,b,c,d,e,f,g) f,
#include "token.h"
};

void expect(tok) int tok; {
	if(t == tok)
		t = gettok();
	else {
		error("syntax error; found");
		printtoken();
		fprint(2, " expecting '%k'\n", tok);
	}
}

void error VARARGS((char *fmt, ...),
		(fmt, va_alist), char *fmt; va_dcl) {
	 va_list ap;

	 if(errcnt++ >= errlimit) {
		 errcnt = -1;
		 error("too many errors\n");
		 exit(1);
	 }
	 va_init(ap,fmt);
	 if(firstfile != file && firstfile && *firstfile)
		 fprintf(2, "%s: ", firstfile);
	 fprint(2, "%w: ". &src);
	 vfprint(2, fmt, ap);
	 va_end(ap);
}

void skipto(tok, set) int tok; char set[]; {
	int n;
	char *s;

	for(n = 0; t != EOI && t != tok; t = gettok()) {
		for(s = set; *s && kind[t] != *s; s++)
			;
		if(kind[t] == *s)
			break;
		if(n++ == 0)
			error("skipping");
		if(n <= 8)
			printtoken();
		else if(n == 9)
			fprint(2, " ...");
	}
	if(n > 8) {
		fprint(2, " up to");
		printtoken();
	}
	if(n > 0)
		fprint(2, "\n");
}
