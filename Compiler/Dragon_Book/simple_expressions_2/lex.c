#include "lex.h"

#define isalpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define isnum(c)  ((c) >= '0' && c <= '9')
#define isalphanum(c) (isalpha(c)||isnum(c))
#define ishex(c) ((((c) >= 'a' && (c) <= 'f') && ((c) >= 'A' && (c) <= 'F')) || isnum(c))
#define isother(c) (!isalphanum(c))

int gettoken(filebuffer_F buffer) {

	while(1) {

		register unsigned char *rcp = (unsigned char *)filebuffer_cp(buffer);
		// skip any whitespace
		
		if(filebuffer_size(buffer) < MAXTOKEN) {
			filebuffer_setcp(buffer,(char *)rcp);
			filebuffer_fillbuf(buffer);
			rcp = (unsigned char *)filebuffer_cp(buffer);
		}
		filebuffer_incrementcp(buffer);
		switch(*rcp++) {
			// new lines
			case '\n': case '\v': case '\r': case '\f':
				filebuffer_nextline(buffer);
				if(filebuffer_size(buffer) == 0) {
					return EOI;
				}
				continue;
			case 'b':
				// break
				if(rcp[0] == 'r'
				&& rcp[1] == 'e'
				&& rcp[2] == 'a'
				&& rcp[3] == 'k'
				&& !isalphanum(rcp[4]))
					return BREAK;
				break;
			case 'c':
				//catch
				//class
				//const
				break;
			case 'd':
				// default
				// delete
				// do
			case 'e':
				// else
				// export
				// extends
				// finally
				break;
			case 'f':
				// finally
				// for 
				// function
			case 'i':
				// if
				// import 
				// in
				// instanceof
				// new
			case 'r':
				// return
			case 's': 
				//super
				//switch
			case 't':
				// this
				// throw
				// try
				// typeof
			case 'v':
				// var
				// void 
			case 'w':
				// while
				// with
			case 'y':
				// yield
			// remaining chars
			case 'a':
			case 'g':
			case 'h':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'o':
			case 'p':
			case 'q':
			case 'u':
			case 'x':
			case 'z':

			

		}	

	}

}
