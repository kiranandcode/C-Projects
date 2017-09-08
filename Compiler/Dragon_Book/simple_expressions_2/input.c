#include "input.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define F filebuffer_F

struct F {
	FILE *fp;
	// buffer contains the currently retrieved file data
	char *buffer;
	// cp points to the current position
	char *cp;
	// limit points to the last position
	char *limit;	

	int lineno;

	char *line;
};



F filebuffer_new(FILE *fp) {
	assert(fp);
	F buffer;
	// generate buffer
	buffer = calloc(sizeof(*buffer), 1);
	assert(buffer);
	buffer->buffer = calloc(sizeof(*buffer->buffer), PREBUFSIZE + 1 + BUFSIZE +1);
	assert(buffer->buffer);
	
	buffer->fp = fp;
	buffer->cp = &buffer->buffer[PREBUFSIZE + 1];
	// read in data
	size_t read = fread(buffer->cp, sizeof(*buffer->buffer),
			BUFSIZE, fp);
	assert(read);
	buffer->limit = &buffer->buffer[PREBUFSIZE + 1 + read];
	*buffer->limit = '\n';
	buffer->line = buffer->cp;
	buffer->lineno = 0;
	return buffer;
}

void filebuffer_delete(F buffer) {
	assert(buffer);
	free(buffer->buffer);
	buffer->buffer = NULL;
	free(buffer);
}


char *filebuffer_nextline(F buffer) {
	assert(buffer);
	
	do {
	        //printf("cp pos %lu, limit pos %lu, lineno %d\n", buffer->cp - buffer->buffer,buffer->limit - buffer->buffer, buffer->lineno);
		// reached end copy over
		if(buffer->cp >= buffer->limit) {
			filebuffer_fillbuf(buffer);
			if(buffer->limit == buffer->cp)
				return buffer->cp;
		}
		else 
			buffer->lineno++;
		// skip whitespace	
		for(buffer->line = (char *)buffer->cp; *buffer->cp == ' ' || *buffer->cp == '\t' || *buffer->cp == '\n'; buffer->cp++)
			;
	} while(*buffer->cp == '\n' || buffer->cp == buffer->limit);
	return buffer->cp;
}

char *filebuffer_fillbuf(F buffer) {
	assert(filebuffer_size(buffer) < PREBUFSIZE);

	//printf("filling buffer\n");
	//printf("cp pos %lu, limit pos %lu, lineno %d\n", buffer->cp - buffer->buffer,buffer->limit - buffer->buffer, buffer->lineno);
	if(buffer->cp >= buffer->limit)
		buffer->cp = &buffer->buffer[PREBUFSIZE + 1];
	else {
		char *curr = buffer->cp;	

		// cp is set to point size back from end of prebuffer
		buffer->cp = buffer->buffer + PREBUFSIZE  + 1 - filebuffer_size(buffer);

		// update line to accurately represent position
		buffer->line = buffer->cp - (buffer->limit - curr);

		char *start = buffer->cp;

		// coppy over the file
		while(curr < buffer->limit) {
			 *start++ = *curr++;
		}

	}

	size_t read = fread(&buffer->buffer[PREBUFSIZE + 1], sizeof(char), BUFSIZE, buffer->fp);

	//printf("cp pos %lu, limit pos %lu, lineno %d\n", buffer->cp - buffer->buffer,buffer->limit - buffer->buffer, buffer->lineno);
	buffer->limit = &buffer->buffer[PREBUFSIZE + 1 + read];

	//printf("cp pos %lu, limit pos %lu, lineno %d\n", buffer->cp - buffer->buffer,buffer->limit - buffer->buffer, buffer->lineno);
	*buffer->limit = '\n';

	return buffer->cp;
}

char *filebuffer_incrementcp(F buffer) {
	if(buffer->cp < buffer->limit)
		buffer->cp++;
	return buffer->cp;
}

void filebuffer_setcp(F buffer, char *cp){ 
	assert(cp >= buffer->buffer &&
	       cp <= buffer->limit);
	buffer->cp = cp;
}

size_t filebuffer_size(F buffer) {
	return buffer->cp >= buffer->limit ? 0 : buffer->limit - buffer->cp;
}

char *filebuffer_cp(F buffer) {
	return buffer->cp;
}

char *filebuffer_limit(F buffer){
	return buffer->limit;
}

int filebuffer_lineno(F buffer) {
	return buffer->lineno;
}

int filebuffer_columnno(F buffer) {
	return buffer->cp - buffer->line;
}

FILE *filebuffer_fp(F buffer) {
	return buffer->fp;
}
