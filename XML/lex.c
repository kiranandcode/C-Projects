#include "lex.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define isalpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define isnum(c)  ((c) >= '0' && c <= '9')
#define isalphanum(c) (isalpha(c)||isnum(c))
#define ishex(c) ((((c) >= 'a' && (c) <= 'f') && ((c) >= 'A' && (c) <= 'F')) || isnum(c))
#define isother(c) (!isalphanum(c))
#define isblank(c) ( (c)==' '|| (c) == '\t')
struct lexer_L  {
	filebuffer_F buffer;
	char *str;
	unsigned int strbuf_size;
	int last_token;
	enum lexer_state {
		DEFAULT = 0,
		ATTR_NAME = 1,
		ATTR_VALUE = 2,
	        CONTENT_VALUE = 3
	} state;
	enum lexer_state prev_state;
};
static int internal_gettoken(lexer_L lexer);

lexer_L lexer_new(filebuffer_F buffer) {
	assert(buffer);
	lexer_L lexer;
	lexer = malloc(sizeof(*lexer));
	assert(lexer);
	lexer->buffer = buffer;
	lexer->str = calloc(sizeof(*lexer->str), 35);
	assert(lexer->str);
	lexer->last_token = -1;
	lexer->strbuf_size = 35;
	lexer->state = DEFAULT;
	lexer->prev_state = DEFAULT;
	return lexer;
}

void lexer_delete(lexer_L lexer) {
	assert(lexer);
	assert(lexer->str);

	free(lexer->str);
	free(lexer);
	return;
}
int lexer_getlasttoken(lexer_L lexer) {
	assert(lexer);
	return lexer->last_token;
}

int lexer_gettoken(lexer_L lexer) {
	assert(lexer);
	int result = internal_gettoken(lexer);
	lexer->last_token = result;
	return result;
}

size_t lexer_lexemelen(lexer_L lexer) {
	assert(lexer);
	assert(lexer->str);
	char *str = lexer->str;
	size_t size = 0;
	while(*str++ != '\0') size++;
	return size;
}
void lexer_lexemecopy(lexer_L lexer, char *buf, size_t size) {
	assert(lexer);
	assert(buf);
	assert(size);

	char *str = lexer->str;
	size_t index = 0;
	while(*str != '\0' && index < size - 1) {
		buf[index++] = *str++;
	}	
	if(*str == '\0') 
		printf("ZERO REACHED\n");
	buf[index] = '\0';
	return;
}

static int internal_gettoken(lexer_L lexer) {
	filebuffer_F buffer = lexer->buffer;

	while(1) {

		register unsigned char *rcp = (unsigned char *)filebuffer_cp(buffer);
		// skip any whitespace
		while(isblank(*rcp))
			rcp++;
		
		if(filebuffer_limit(buffer) - (char *)rcp < MAXTOKEN) {
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
			case '<':
				// comments
				if(rcp[0] == '!'
				&& rcp[1] == '-'
				&& rcp[2] == '-'){
					rcp = rcp + 3;
					while(1) {
						while((*rcp != '-') && (*rcp != '\n') && (*rcp != '\v') && (*rcp != '\f')) rcp++;
						if(*rcp == '\n' || *rcp == '\v' || *rcp == '\f' || *rcp == '\r'){
							filebuffer_setcp(buffer, (char *)rcp);
							filebuffer_fillbuf(buffer);
							rcp = (unsigned char *)filebuffer_cp(buffer);
							continue;
						}
						if(filebuffer_limit(buffer) - (char *) rcp < MAXTOKEN) {
							filebuffer_setcp(buffer,(char *)rcp);
							filebuffer_fillbuf(buffer);
							rcp = (unsigned char *)filebuffer_cp(buffer);
						}
						if(*rcp == '-')  {
							if(rcp[1] == '-' && rcp[2] == '>') {
								filebuffer_setcp(buffer, (char *)rcp + 3);
								break;
							} else {
								rcp++;
							}
						}
					}
					continue;
			

				} 
				else if (rcp[0] == '/'){
					// tag close </NAME>
					lexer->state = lexer->prev_state;
					if(filebuffer_limit(buffer) - (char *)rcp < MAXTOKEN) {
							filebuffer_setcp(buffer, (char *)rcp);
							filebuffer_fillbuf(buffer);
							rcp = (unsigned char *)filebuffer_cp(buffer);
					}
					char *start = (char *)rcp +1;
					while(*rcp != '>' && *rcp != '\n') rcp++;
					int has_ending = (*rcp == '\n' ? 0 : 1);
					char *end = (char *)rcp;
					if((start - (char *)end) > lexer->strbuf_size) {
						lexer->str = realloc(lexer->str, sizeof(*lexer->str) * ((end - (char *)start) + 10));
						lexer->strbuf_size = (end - (char *)start) + 10;
					}
					int index;
					// copy over the non blank chars into the str name
					for(index = 0; (!isblank(start[index])) && start[index] != '\n' && start[index] != '>' && index < (end - (char *)start); index++) {
						//printf("Copying over %c into str[%d]\n", start[index], index);
						lexer->str[index] = start[index];
					}
					lexer->str[index] = '\0';

					// if </name  is before a newline, keep on retrieving lines until >, subsequent chars have
					// been ignored
					if(!has_ending) {
						while(1) {
							while((*rcp != '>') && (*rcp != '\n') && (*rcp != '\v') && (*rcp != '\f')) {
								rcp++;
							}
							if(*rcp == '\n' || *rcp == '\v' || *rcp == '\f' || *rcp == '\r'){
								filebuffer_setcp(buffer, (char *)rcp);
								filebuffer_fillbuf(buffer);
								rcp = (unsigned char *)filebuffer_cp(buffer);
								if(*rcp == '\n' || *rcp == '\v' || *rcp == '\f' || *rcp == '\r'){
									rcp++;
								}
								continue;
							}
							if(filebuffer_limit(buffer) - (char *)rcp < MAXTOKEN) {
								filebuffer_setcp(buffer,(char *)rcp);
								filebuffer_fillbuf(buffer);
								rcp = (unsigned char *)filebuffer_cp(buffer);
							}
							if(*rcp == '>') break;
						}
					} 

					filebuffer_setcp(buffer, (char *)rcp + 1);
					return TAGCLOSE;
				}
				else {
					// tagbegin <NAME
					lexer->prev_state = lexer->state;
					lexer->state = ATTR_NAME;
					if(filebuffer_limit(buffer) - (char *) rcp < MAXTOKEN) {
							filebuffer_setcp(buffer, (char *)rcp);
							filebuffer_fillbuf(buffer);
							rcp = (unsigned char *)filebuffer_cp(buffer);
					}
					char *start = (char *)rcp;
					while(!isblank(*rcp) && *rcp != '>' && *rcp != '/' && *rcp != '\n') rcp++;
					char *end = (char *)rcp;
					if((end - (char *)start) > lexer->strbuf_size) {
						lexer->str = realloc(lexer->str,sizeof(*lexer->str) * (end - (char *)start) + 10);
						lexer->strbuf_size = (end - (char *)start) + 10;
					}
					int index;
					// copy over the non blank chars into the str name
					for(index = 0; !isblank(start[index]) && start[index] != '>'  && index < (end - (char *)start); index++) {
						lexer->str[index] = start[index];
					}
					lexer->str[index] = '\0';
					

					filebuffer_setcp(buffer, (char *)rcp);
					return TAGBEGIN;
				}
			case '/':
				if(rcp[0] == '>') {
					// tagendandclose
					filebuffer_setcp(lexer->buffer, (char *)rcp + 1);
					lexer->state = lexer->prev_state;
					return TAGENDANDCLOSE;
				} else {
					goto id;
				}
			case '>':
				// tagend
				filebuffer_setcp(lexer->buffer, (char *)rcp);
				lexer->state = CONTENT_VALUE;
				return TAGEND;
			case '=':
				// equalsign
				if(lexer->state != ATTR_NAME) {
					goto id;
				}
				filebuffer_setcp(lexer->buffer, (char *)rcp);
				lexer->state = ATTR_VALUE;
				return EQUALSIGN;
				


			default:
			id: {
			    	char *start, *end;
			    	int index;
				switch(lexer->state) {
					case DEFAULT:
						// identifier when in default state - probably missing <
						// thus interpret as <NAME  - tagbegin
						rcp = rcp - 1;
						index = 0;
						if(filebuffer_limit(buffer) - (char *)rcp < MAXTOKEN) {
							filebuffer_setcp(buffer, (char *)rcp);
							filebuffer_fillbuf(buffer);
							rcp = (unsigned char *)filebuffer_cp(buffer);
						}
						start = (char *)rcp;
						while((*rcp != '=') && (*rcp != '/') && (*rcp != '>') && *rcp != '\n') {
							lexer->str[index++] = *rcp++;
							if(filebuffer_limit(buffer) - (char *)rcp < MAXTOKEN) {
								filebuffer_setcp(buffer, (char *)rcp);
								filebuffer_fillbuf(buffer);
								rcp = (unsigned char *)filebuffer_cp(buffer);
						       	}
							if((unsigned int)index > lexer->strbuf_size) {
								lexer->str = realloc(lexer->str, sizeof(*lexer->str) * (index + 10));
								lexer->strbuf_size = index + 10;
							}	

						}
						//end = (char *)rcp;
						if((unsigned int)index > lexer->strbuf_size) {
							lexer->str = realloc(lexer->str, sizeof(*lexer->str) *( index + 10));
							lexer->strbuf_size = index + 10;
						}	
					
						// copy over the non blank chars into the str name
/*						for(index = 0; !isblank(start[index]) && index < (end - (char *)start); index++) {
							lexer->str[index] = start[index];
						}*/
						lexer->str[index] = '\0';

						filebuffer_setcp(buffer, (char *)rcp);
						lexer->prev_state = lexer->state;	
						lexer->state = ATTR_NAME;
						return TAGBEGIN;
					case CONTENT_VALUE:
						// content value - content value
						//
						index = 0;
						rcp = rcp - 1;
						if(filebuffer_limit(buffer) - (char *)rcp < MAXTOKEN) {
							filebuffer_setcp(buffer, (char *)rcp);
							filebuffer_fillbuf(buffer);
							rcp = (unsigned char *)filebuffer_cp(buffer);
						}
//						start = (char *)rcp;
						while((*rcp != '<') && *rcp != '\n') {
							if(*rcp == '\\' && *(rcp + 1) == '<') {
//								printf("Escape Char %c\n", *rcp);
								lexer->str[index++] = *rcp++;
							}
//							printf("Char %c\n", *rcp);
							lexer->str[index++] = *rcp++;

							if(filebuffer_limit(buffer)-(char *)rcp < MAXTOKEN) {
								filebuffer_setcp(buffer, (char *)rcp);
								filebuffer_fillbuf(buffer);
								rcp = (unsigned char *)filebuffer_cp(buffer);
							}
							if((unsigned int)index >= lexer->strbuf_size) {
								lexer->str = realloc(lexer->str, sizeof(*lexer->str) * (index + 10));
								lexer->strbuf_size = index + 10;
							}	
						}
						//end = (char *)rcp;
						//if((end - (char *)start) > lexer->strbuf_size) {
						//	lexer->str = realloc(lexer->str, (end - (char *)start) + 10);
						//	lexer->strbuf_size = (end - (char *)start) + 10;
						//}
						// copy over the non blank chars into the str name
						/*for(index = 0; index < (end - (char *)start); index++) {
							lexer->str[index] = start[index];
						}*/

						if(*rcp != '<')
						while(*rcp != '<') {
							printf("Infinite while 2 entered again\n");
							while((*rcp != '<') && (*rcp != '\n') && (*rcp != '\v') && (*rcp != '\f')) {

								if(*rcp == '\\' && *(rcp + 1) == '<') {
									printf("2nd Unsigned Char %c\n", *rcp);
									lexer->str[index++] = *rcp++;
								}

								printf("2nd Char %c\n", *rcp);

								lexer->str[index++] = *rcp++;

								if(filebuffer_limit(buffer)-(char *)rcp < MAXTOKEN) {
									filebuffer_setcp(buffer, (char *)rcp);
									filebuffer_fillbuf(buffer);
									rcp = (unsigned char *)filebuffer_cp(buffer);
								}

								if((unsigned int)index >= lexer->strbuf_size) {
									lexer->str = realloc(lexer->str, sizeof(*lexer->str) * (index + 10));
									lexer->strbuf_size = index + 10;
								}	
							}
							if(*rcp == '\n' || *rcp == '\v' || *rcp == '\f' || *rcp == '\r'){
								printf("newline reached\n");
								filebuffer_setcp(buffer, (char *)rcp);
								filebuffer_fillbuf(buffer);
								rcp = (unsigned char *)filebuffer_cp(buffer);

								lexer->str[index++] = '\n';
								if((unsigned int)index > lexer->strbuf_size) {
									lexer->str = realloc(lexer->str, sizeof(*lexer->str) * (index + 10));
									lexer->strbuf_size = index + 10;
								}	
								//printf("rcp is %c, rcp++ is %c\n", *rcp, *(rcp + 1));
								rcp++;
								continue;
							}
							if(filebuffer_limit(buffer) - (char *)rcp < MAXTOKEN) {
								filebuffer_setcp(buffer,(char *)rcp);
								filebuffer_fillbuf(buffer);
								rcp = (unsigned char *)filebuffer_cp(buffer);
							}

//							start = (char *)rcp;

							if(*rcp == '<')  {
								filebuffer_setcp(buffer, (char *)rcp);
								break;
							} 
						}


						lexer->str[index] = '\0';

						filebuffer_setcp(buffer, (char *)rcp);

						return CONTENT;
						break;
					case ATTR_VALUE:
						lexer->state = ATTR_NAME;
				rcp = rcp - 1;
				if(filebuffer_limit(buffer) - (char *)rcp < MAXTOKEN) {
						filebuffer_setcp(buffer, (char *)rcp);
						filebuffer_fillbuf(buffer);
						rcp = (unsigned char *)filebuffer_cp(buffer);
				}
				start = (char *)rcp;
				while((*rcp != '=') && (*rcp != '/') && (*rcp != '>') && *rcp != '\n') rcp++;
				end = (char *)rcp;
				if((end - (char *)start) > lexer->strbuf_size) {
					lexer->str = realloc(lexer->str, sizeof(*lexer->str) * ((end - (char *)start) + 10));
					lexer->strbuf_size = (end - (char *)start) + 10;
				}
				index = 0;
				// copy over the non blank chars into the str name
				for(index = 0; !isblank(start[index]) && index < (end - (char *)start); index++) {
					lexer->str[index] = start[index];
				}
				lexer->str[index] = '\0';

				filebuffer_setcp(buffer, (char *)rcp);

						return ATTRIBUTEVALUE;
						break;
					case ATTR_NAME:
				rcp = rcp - 1;
				if(filebuffer_limit(buffer) - (char *)rcp < MAXTOKEN) {
						filebuffer_setcp(buffer, (char *)rcp);
						filebuffer_fillbuf(buffer);
						rcp = (unsigned char *)filebuffer_cp(buffer);
				}
				start = (char *)rcp;
				while((*rcp != '=') && (*rcp != '/') && (*rcp != '>') && *rcp != '\n') rcp++;
				end = (char *)rcp;
				if((end - (char *)start) > lexer->strbuf_size) {
					lexer->str = realloc(lexer->str, sizeof(*lexer->str) * ((end - (char *)start) + 10));
					lexer->strbuf_size = (end - (char *)start) + 10;
				}
				index = 0;
				// copy over the non blank chars into the str name
				for(index = 0; !isblank(start[index]) && index < (end - (char *)start); index++) {
					lexer->str[index] = start[index];
				}
				lexer->str[index] = '\0';

				filebuffer_setcp(buffer, (char *)rcp);

						return ATTRIBUTENAME;
						break;
				}
			    }
		}	

	}

}
