#include "lex.h"

#define isalpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define isnum(c)  ((c) >= '0' && c <= '9')
#define isalphanum(c) (isalpha(c)||isnum(c))
#define ishex(c) ((((c) >= 'a' && (c) <= 'f') && ((c) >= 'A' && (c) <= 'F')) || isnum(c))
#define isother(c) (!isalphanum(c))
#define isblank(c) ( (c)==' '|| (c) == '\t')

int gettoken(filebuffer_F buffer) {

	while(1) {

		register unsigned char *rcp = (unsigned char *)filebuffer_cp(buffer);
		// skip any whitespace
		while(isblank(*rcp))
			rcp++;
		
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
			case '/':
				// comments
				if(rcp[0] == '*'){
					rcp++;
					while(1) {
while((*rcp != '*') && (*rcp != '\n') && (*rcp != '\v') && (*rcp != '\f')) rcp++;
if(*rcp == '\n' || *rcp == '\v' || *rcp == '\f' || *rcp == '\r'){
	filebuffer_setcp(buffer, (char *)rcp);
	filebuffer_fillbuf(buffer);
	continue;
}
	if(filebuffer_size(buffer) < MAXTOKEN) {
			filebuffer_setcp(buffer,(char *)rcp);
			filebuffer_fillbuf(buffer);
			rcp = (unsigned char *)filebuffer_cp(buffer);
	}
	if(*rcp == '*' && *(rcp + 1) == '/') {
		filebuffer_setcp(buffer, (char *)rcp + 2);
		break;
	}
					}
					continue;
			

				}
				if(rcp[0] == '/') {

					while(1) {
						while((*rcp != '\n') && (*rcp != '\v') && (*rcp != '\f')) rcp++;
						filebuffer_setcp(buffer, (char *) rcp);
						if(filebuffer_size(buffer) == 0) {
							// fillbuffer continue
							filebuffer_fillbuf(buffer);
							continue;
						}
						break;
					}
					continue;
				}

			case 'a':
				//assert
				if(rcp[0] == 's' &&
				   rcp[1] == 's' &&
				   rcp[2] == 'e' &&
				   rcp[3] == 'r' &&
				   rcp[4] == 't' &&
				   !isalphanum(rcp[5])){
					filebuffer_setcp(buffer,(char *) rcp + 5);
					return ASSERT;
				}
				//abstract
				if(rcp[0] == 'b' &&
				   rcp[1] == 's' &&
				   rcp[2] == 't' &&
				   rcp[3] == 'r' &&
				   rcp[4] == 'a' &&
				   rcp[5] == 'c' &&
				   rcp[6] == 't' &&
				   !isalphanum(rcp[7])) {
					filebuffer_setcp(buffer,
							(char *) rcp + 7);
					return ABSTRACT;
				}
				goto id;

			case 'b':
			// boolean
if( rcp[0] == 'o'  && rcp[1] == 'o'  && rcp[2] == 'l'  && rcp[3] == 'e'  && rcp[4] == 'a'  && rcp[5] == 'n'  && !isalphanum(rcp[6])) {
filebuffer_setcp(buffer, (char *)rcp+6);
return BOOLEAN;
}
			// break
if( rcp[0] == 'r'  && rcp[1] == 'e'  && rcp[2] == 'a'  && rcp[3] == 'k'  && !isalphanum(rcp[4])) {
filebuffer_setcp(buffer, (char *)rcp+4);
return BREAK;
}
			// byte
if( rcp[0] == 'y'  && rcp[1] == 't'  && rcp[2] == 'e'  && !isalphanum(rcp[3])) {
filebuffer_setcp(buffer, (char *)rcp+3);
return BYTE;
}
goto id;
				

			case 'c':
				// case
				if(rcp[0] == 'a' &&
				   rcp[1] == 's' &&
				   rcp[2] == 'e' &&
				   !isalphanum(rcp[3])){
					filebuffer_setcp(buffer, (char *)rcp + 3);
					return CASE;
				}
				// char
if( rcp[0] == 'h'  && rcp[1] == 'a'  && rcp[2] == 'r'  && !isalphanum(rcp[3])) {
filebuffer_setcp(buffer, (char *)rcp+3);
return CHAR;
}
				// catch
if( rcp[0] == 'a'  && rcp[1] == 't'  && rcp[2] == 'c'  && rcp[3] == 'h'  && !isalphanum(rcp[4])) {
filebuffer_setcp(buffer, (char *)rcp+4);
return CATCH;
}
				// class
if( rcp[0] == 'l'  && rcp[1] == 'a'  && rcp[2] == 's'  && rcp[3] == 's'  && !isalphanum(rcp[4])) {
filebuffer_setcp(buffer, (char *)rcp+4);
return CLASS;
}
				// const
if( rcp[0] == 'o'  && rcp[1] == 'n'  && rcp[2] == 's'  && rcp[3] == 't'  && !isalphanum(rcp[4])) {
filebuffer_setcp(buffer, (char *)rcp+4);
return CONST;
}
				// continue
if( rcp[0] == 'o'  && rcp[1] == 'n'  && rcp[2] == 't'  && rcp[3] == 'i'  && rcp[4] == 'n'  && rcp[5] == 'u'  && rcp[6] == 'e'  && !isalphanum(rcp[7])) {
filebuffer_setcp(buffer, (char *)rcp+7);
return CONTINUE;
}
goto id;
			case 'd':
			// default
if( rcp[0] == 'e'  && rcp[1] == 'f'  && rcp[2] == 'a'  && rcp[3] == 'u'  && rcp[4] == 'l'  && rcp[5] == 't'  && !isalphanum(rcp[6])) {
filebuffer_setcp(buffer, (char *)rcp+6);
return DEFAULT;
}
			// double
if( rcp[0] == 'o'  && rcp[1] == 'u'  && rcp[2] == 'b'  && rcp[3] == 'l'  && rcp[4] == 'e'  && !isalphanum(rcp[5])) {
filebuffer_setcp(buffer, (char *)rcp+5);
return DOUBLE;
}
			// do
if( rcp[0] == 'o'  && !isalphanum(rcp[1])) {
filebuffer_setcp(buffer, (char *)rcp+1);
return DO;
}
goto id;
			case 'e':
			// else
if( rcp[0] == 'l'  && rcp[1] == 's'  && rcp[2] == 'e'  && !isalphanum(rcp[3])) {
filebuffer_setcp(buffer, (char *)rcp+3);
return ELSE;
}
			// enum
if( rcp[0] == 'n'  && rcp[1] == 'u'  && rcp[2] == 'm'  && !isalphanum(rcp[3])) {
filebuffer_setcp(buffer, (char *)rcp+3);
return ENUM;
}
			// extends
if( rcp[0] == 'x'  && rcp[1] == 't'  && rcp[2] == 'e'  && rcp[3] == 'n'  && rcp[4] == 'd'  && rcp[5] == 's'  && !isalphanum(rcp[6])) {
filebuffer_setcp(buffer, (char *)rcp+6);
return EXTENDS;
}
goto id;
			case 'f':
			// false
			// ------------------------------------------
			// TODO ( add false implementation)
			//-------------------------------------------
			// finally
if( rcp[0] == 'i'  && rcp[1] == 'n'  && rcp[2] == 'a'  && rcp[3] == 'l'  && rcp[4] == 'l'  && rcp[5] == 'y'  && !isalphanum(rcp[6])) {
filebuffer_setcp(buffer, (char *)rcp+6);
return FINALLY;
}
			// final
if( rcp[0] == 'i'  && rcp[1] == 'n'  && rcp[2] == 'a'  && rcp[3] == 'l'  && !isalphanum(rcp[4])) {
filebuffer_setcp(buffer, (char *)rcp+4);
return FINAL;
}
			// float
if( rcp[0] == 'l'  && rcp[1] == 'o'  && rcp[2] == 'a'  && rcp[3] == 't'  && !isalphanum(rcp[4])) {
filebuffer_setcp(buffer, (char *)rcp+4);
return FLOAT;
}
			// for
if( rcp[0] == 'o'  && rcp[1] == 'r'  && !isalphanum(rcp[2])) {
filebuffer_setcp(buffer, (char *)rcp+2);
return FOR;
}
goto id;
			case 'g':
			// goto
if( rcp[0] == 'o'  && rcp[1] == 't'  && rcp[2] == 'o'  && !isalphanum(rcp[3])) {
filebuffer_setcp(buffer, (char *)rcp+3);
return GOTO;
}
goto id;
			case 'i':
			// implements
if( rcp[0] == 'm'  && rcp[1] == 'p'  && rcp[2] == 'l'  && rcp[3] == 'e'  && rcp[4] == 'm'  && rcp[5] == 'e'  && rcp[6] == 'n'  && rcp[7] == 't'  && rcp[8] == 's'  && !isalphanum(rcp[9])) {
filebuffer_setcp(buffer, (char *)rcp+9);
return IMPLEMENTS;
}
			// instanceof
if( rcp[0] == 'n'  && rcp[1] == 's'  && rcp[2] == 't'  && rcp[3] == 'a'  && rcp[4] == 'n'  && rcp[5] == 'c'  && rcp[6] == 'e'  && rcp[7] == 'o'  && rcp[8] == 'f'  && !isalphanum(rcp[9])) {
filebuffer_setcp(buffer, (char *)rcp+9);
return INSTANCEOF;
}
			// import
if( rcp[0] == 'm'  && rcp[1] == 'p'  && rcp[2] == 'o'  && rcp[3] == 'r'  && rcp[4] == 't'  && !isalphanum(rcp[5])) {
filebuffer_setcp(buffer, (char *)rcp+5);
return IMPORT;
}
			// int
if( rcp[0] == 'n'  && rcp[1] == 't'  && !isalphanum(rcp[2])) {
filebuffer_setcp(buffer, (char *)rcp+2);
return INT;
}
			// interface
if( rcp[0] == 'n'  && rcp[1] == 't'  && rcp[2] == 'e'  && rcp[3] == 'r'  && rcp[4] == 'f'  && rcp[5] == 'a'  && rcp[6] == 'c'  && rcp[7] == 'e'  && !isalphanum(rcp[8])) {
filebuffer_setcp(buffer, (char *)rcp+8);
return INTERFACE;
}
			// if
if( rcp[0] == 'f'  && !isalphanum(rcp[1])) {
filebuffer_setcp(buffer, (char *)rcp+1);
return IF;
}
goto id;
			case 'l':
			// long
if( rcp[0] == 'o'  && rcp[1] == 'n'  && rcp[2] == 'g'  && !isalphanum(rcp[3])) {
filebuffer_setcp(buffer, (char *)rcp+3);
return LONG;
}
goto id;
			case 'n':
			// native
if( rcp[0] == 'a'  && rcp[1] == 't'  && rcp[2] == 'i'  && rcp[3] == 'v'  && rcp[4] == 'e'  && !isalphanum(rcp[5])) {
filebuffer_setcp(buffer, (char *)rcp+5);
return NATIVE;
}
			// new
if( rcp[0] == 'e'  && rcp[1] == 'w'  && !isalphanum(rcp[2])) {
filebuffer_setcp(buffer, (char *)rcp+2);
return NEW;
}
			// null
if( rcp[0] == 'u'  && rcp[1] == 'l'  && rcp[2] == 'l'  && !isalphanum(rcp[3])) {
filebuffer_setcp(buffer, (char *)rcp+3);
return NULL_LITERAL;
}
goto id;
			case 'p':
				// package
if( rcp[0] == 'a'  && rcp[1] == 'c'  && rcp[2] == 'k'  && rcp[3] == 'a'  && rcp[4] == 'g'  && rcp[5] == 'e'  && !isalphanum(rcp[6])) {
filebuffer_setcp(buffer, (char *)rcp+6);
return PACKAGE;
}
				// private
if( rcp[0] == 'r'  && rcp[1] == 'i'  && rcp[2] == 'v'  && rcp[3] == 'a'  && rcp[4] == 't'  && rcp[5] == 'e'  && !isalphanum(rcp[6])) {
filebuffer_setcp(buffer, (char *)rcp+6);
return PRIVATE;
}
				// protected
if( rcp[0] == 'r'  && rcp[1] == 'o'  && rcp[2] == 't'  && rcp[3] == 'e'  && rcp[4] == 'c'  && rcp[5] == 't'  && rcp[6] == 'e'  && rcp[7] == 'd'  && !isalphanum(rcp[8])) {
filebuffer_setcp(buffer, (char *)rcp+8);
return PROTECTED;
}
				// public
if( rcp[0] == 'u'  && rcp[1] == 'b'  && rcp[2] == 'l'  && rcp[3] == 'i'  && rcp[4] == 'c'  && !isalphanum(rcp[5])) {
filebuffer_setcp(buffer, (char *)rcp+5);
return PUBLIC;
}
goto id;
			case 'r':
			// return
if( rcp[0] == 'e'  && rcp[1] == 't'  && rcp[2] == 'u'  && rcp[3] == 'r'  && rcp[4] == 'n'  && !isalphanum(rcp[5])) {
filebuffer_setcp(buffer, (char *)rcp+5);
return RETURN;
}
goto id;
			case 's':
			// short
if( rcp[0] == 'h'  && rcp[1] == 'o'  && rcp[2] == 'r'  && rcp[3] == 't'  && !isalphanum(rcp[4])) {
filebuffer_setcp(buffer, (char *)rcp+4);
return SHORT;
}
			// static
if( rcp[0] == 't'  && rcp[1] == 'a'  && rcp[2] == 't'  && rcp[3] == 'i'  && rcp[4] == 'c'  && !isalphanum(rcp[5])) {
filebuffer_setcp(buffer, (char *)rcp+5);
return STATIC;
}
			// strictfp
if( rcp[0] == 't'  && rcp[1] == 'r'  && rcp[2] == 'i'  && rcp[3] == 'c'  && rcp[4] == 't'  && rcp[5] == 'f'  && rcp[6] == 'p'  && !isalphanum(rcp[7])) {
filebuffer_setcp(buffer, (char *)rcp+7);
return STRICTFP;
}
			// super
if( rcp[0] == 'u'  && rcp[1] == 'p'  && rcp[2] == 'e'  && rcp[3] == 'r'  && !isalphanum(rcp[4])) {
filebuffer_setcp(buffer, (char *)rcp+4);
return SUPER;
}
			// switch
if( rcp[0] == 'w'  && rcp[1] == 'i'  && rcp[2] == 't'  && rcp[3] == 'c'  && rcp[4] == 'h'  && !isalphanum(rcp[5])) {
filebuffer_setcp(buffer, (char *)rcp+5);
return SWITCH;
}
			// synchronized
if( rcp[0] == 'y'  && rcp[1] == 'n'  && rcp[2] == 'c'  && rcp[3] == 'h'  && rcp[4] == 'r'  && rcp[5] == 'o'  && rcp[6] == 'n'  && rcp[7] == 'i'  && rcp[8] == 'z'  && rcp[9] == 'e'  && rcp[10] == 'd'  && !isalphanum(rcp[11])) {
filebuffer_setcp(buffer, (char *)rcp+11);
return SYNCHRONIZED;
}
goto id;
			case 't':
			// this
if( rcp[0] == 'h'  && rcp[1] == 'i'  && rcp[2] == 's'  && !isalphanum(rcp[3])) {
filebuffer_setcp(buffer, (char *)rcp+3);
return THIS;
}
			// throw
if( rcp[0] == 'h'  && rcp[1] == 'r'  && rcp[2] == 'o'  && rcp[3] == 'w'  && !isalphanum(rcp[4])) {
filebuffer_setcp(buffer, (char *)rcp+4);
return THROW;
}
			// throws
if( rcp[0] == 'h'  && rcp[1] == 'r'  && rcp[2] == 'o'  && rcp[3] == 'w'  && rcp[4] == 's'  && !isalphanum(rcp[5])) {
filebuffer_setcp(buffer, (char *)rcp+5);
return THROWS;
}
			// transient
if( rcp[0] == 'r'  && rcp[1] == 'a'  && rcp[2] == 'n'  && rcp[3] == 's'  && rcp[4] == 'i'  && rcp[5] == 'e'  && rcp[6] == 'n'  && rcp[7] == 't'  && !isalphanum(rcp[8])) {
filebuffer_setcp(buffer, (char *)rcp+8);
return TRANSIENT;
}
			// try
if( rcp[0] == 'r'  && rcp[1] == 'y'  && !isalphanum(rcp[2])) {
filebuffer_setcp(buffer, (char *)rcp+2);
return TRY;
}
			// true
			// TODO(implement true false)
goto id;
			case 'v':
			// volatile
if( rcp[0] == 'o'  && rcp[1] == 'l'  && rcp[2] == 'a'  && rcp[3] == 't'  && rcp[4] == 'i'  && rcp[5] == 'l'  && rcp[6] == 'e'  && !isalphanum(rcp[7])) {
filebuffer_setcp(buffer, (char *)rcp+7);
return VOLATILE;
}
			// void
if( rcp[0] == 'o'  && rcp[1] == 'i'  && rcp[2] == 'd'  && !isalphanum(rcp[3])) {
filebuffer_setcp(buffer, (char *)rcp+3);
return VOID;
}
goto id;
			case 'w':
			// while
if( rcp[0] == 'h'  && rcp[1] == 'i'  && rcp[2] == 'l'  && rcp[3] == 'e'  && !isalphanum(rcp[4])) {
filebuffer_setcp(buffer, (char *)rcp+4);
return WHILE;
}
goto id;
			case 'h':
			case 'j':
			case 'k':
			case 'm':
			case 'o':
			case 'q':
			case 'u':
			case 'x':
			case 'y':
			case 'z':
			id:
				// ensure there are as many characters as possible
				if(filebuffer_size(buffer) < PREBUFSIZE) {
				filebuffer_setcp(buffer, (char *) rcp - 1);
				filebuffer_fillbuf(buffer);
				filebuffer_incrementcp(buffer);
				rcp = (unsigned char *) filebuffer_cp(buffer);
				}


			

		}	

	}

}
