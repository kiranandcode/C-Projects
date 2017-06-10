#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

int getword(FILE *, char *, int);
void doubleword(char *, FILE *);

int linenum;


int getword(FILE *fp, char *buf, int size) {

	int c;

	c = getc(fp);

	// Skip whitespace
	for(;c != EOF && isspace(c); c = getc(fp))
		if (c == '\n') linenum++;

	// Copy word into buffer
	{
		int i = 0;
		for(; c != EOF && !isspace(c); c = getc(fp)) {
			if(i < size - 1)
				buf[i++] = tolower(c);
		}

		// Handle's case when size is 0. Defensive programming
		if(i < size)
			buf[i] = '\0';
	}

	if(c != EOF)
		ungetc(c,fp);

	return buf[0] != '\0';

}


void doubleword(char *name, FILE *fp) {

	char prev[128], word[128];

	linenum = 1;
	prev[0] = '\0';
	int seen = 0;
	while(getword(fp, word, sizeof(word))) {
		if(isalpha(word[0]) && strcmp(prev, word) == 0) {
		
			if(!seen) {
				if(name)
				printf("%s:", name);
				printf("%d: %s\n", linenum, word);			
			
				seen = 1;
			}

		} else {
			strcpy(prev, word);
			seen = 0;
		}
	}

}


int main(int argc,  char *argv[]) {
	int i;

	for(i = 1; i<argc; i++) {

			FILE *fp = fopen(argv[i], "r");
			if(fp == NULL) {
				fprintf(stderr, "%s: can't open '%s' (%s)\n",
						argv[0], argv[i], strerror(errno));
				return EXIT_FAILURE;
			} else {
				doubleword(argv[i], fp);
				fclose(fp);
			}
	}

	if(argc == 1) doubleword(NULL, stdin);
	return EXIT_SUCCESS;
}
