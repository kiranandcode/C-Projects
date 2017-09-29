#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef struct shader_G *shader_G;
struct shader_G {
	GLuint handle;
};


shader_G shader_new(GLuint type, const GLchar *code) {
	assert(code);
	FILE *errout = stdout;
	GLuint handle = glCreateShader(type);
	glShaderSource(handle,1, &code, NULL);
	glCompileShader(handle);
	GLint isCompiled = 0;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &isCompiled);

	if(isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar buf[maxLength];
		glGetShaderInfoLog(handle, maxLength, &maxLength, buf);
		fprintf(errout, "[ERROR]: shader err: %s\n", buf);
		glDeleteShader(type);
		return NULL;
	}
	shader_G shader = malloc(sizeof(*shader));
	assert(shader);
	shader->handle = handle;
	return shader;
}

void shader_delete(shader_G shader) {
	assert(shader);
	glDeleteShader(shader->handle);
	free(shader);
	return;
}

GLuint shader_get(shader_G shader) {
	assert(shader);
	return shader->handle;
}

