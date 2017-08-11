#ifndef LEXER_H
#define LEXER_H

#define LEXER_BUFFER_SIZE 50

extern int token;
extern union tokenval {
	int value;
	struct entry *ptr_value;
} tokenval;
extern int linenum;
extern int columnnum;

int lexan();

#endif //LEXER_H
