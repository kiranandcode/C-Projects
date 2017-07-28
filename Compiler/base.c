#include "base.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char Look;

// reads another character from input stream
void GetChar() {
	Look = getc(stdin);
}

// Throws an error
void Error(char *string) {
	fprintf(stderr, "Error: %s\n", string);
}

// Returns Error, and exits
void Abort(char *string) {
	Error(string);
	exit(-1);
}

// Reports what was expected
void Expected(char *string) {
	char new[strlen(string) + 11];
	strcpy(new, string);
	strcat(new, " Expected\n");
	Abort(new);
}

// Matches a single character
void Match(char c) {
	if(Look == c) GetChar();
	else {
		char str[2];
		str[0] = c;
		str[1] = '\n';
		Expected(str);
	}	
}


// Checks for Alpha
int IsAlpha(char c) {
	int result = toupper(c);	
	if(result >= 'A' && result <='Z') return 1;
	else return 0;
}

// Checks for Digits
int IsDigit(char c) {
	if(c >= '0' && c <= '9')
		return 1;
	else return 0;
}


// Retrieves Identifier
char GetName() {
	char result;
	if(!IsAlpha(Look)) Expected("Name");
	result = toupper(Look);
	GetChar();
	return result;
}

// Retrieves Number
char GetNum() {
	char result;
	if(!IsDigit(Look)) Expected("Integer");
	result = Look;
	GetChar();
	return result;
}

// Output string with tab
void Emit(char *string) {
	printf("\t%s",string);
}

// Output a string with Tab and newline
void EmitLn(char *string) {
	printf("\t%s\n", string);
}

void Init() {
	GetChar();
}

int IsAddop(char c) {
	if(c == '+' || c == '-') return 1;
	else return 0;
}
