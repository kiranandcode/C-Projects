#include "shaderloader.h"

#include <stdio.h>
#include <assert.h>

#define MAXBUFSIZE 2000


static GLchar buf_free[MAXBUFSIZE];


const GLchar *loadshader(const char *filename) {
	FILE *fp = fopen(filename, "r");
	assert(fp);
	fseek(fp, 0L, SEEK_END);
	size_t size = ftell(fp);
	rewind(fp);
	if(size < MAXBUFSIZE) {
		fread((char *)&buf_free[0], sizeof(GLchar), size,fp);
	}
	buf_free[size] = '\0';
	fclose(fp);
	assert(size < MAXBUFSIZE);
	return buf_free;
}
