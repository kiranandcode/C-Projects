#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"


int logger_restart() {
	FILE *fp = fopen(GL_LOG_FILE, "w");
	if(!fp) {
		fprintf(stderr,"ERROR: could not open GL_LOG_FILE(%s) for writing\n", GL_LOG_FILE);
		return 0;
	}
	time_t now = time(NULL);
	char *date = ctime(&now);
	fprintf(fp, "GL_LOG_FILE log. %s\n", date);
	fclose(fp);
	return 1;
}

int logger_log(const char *message, ...) {
	va_list argptr;
	FILE *fp = fopen(GL_LOG_FILE, "a");
	if(!fp) {
		fprintf(stderr,"ERROR: could not open GL_LOG_FILE(%s) for writing\n", GL_LOG_FILE);
		return 0;
	}

	time_t now; 
	struct tm *timeinfo;
	char buf[80];
	
	time(&now);
	timeinfo = localtime(&now);

	strftime(buf, 80, "%d/%m/%y:%R", timeinfo);
	fprintf(fp, "[%s]: ", buf);
	va_start(argptr, message);
	vfprintf(fp, message, argptr);
	va_end(argptr);

	fclose(fp);
	return 1;
}
int logger_err(const char *message, ...) {
	va_list argptr;
	FILE *fp = fopen(GL_LOG_FILE, "a");
	if(!fp) {
		fprintf(stderr,"ERROR: could not open GL_LOG_FILE(%s) for writing\n", GL_LOG_FILE);
		return 0;
	}
	time_t now; 
	struct tm *timeinfo;
	char buf[80];
	
	time(&now);
	timeinfo = localtime(&now);

	strftime(buf, 80, "%d/%m/%y:%R", timeinfo);
	fprintf(fp, "[%s]: ", buf);

	va_start(argptr, message);
	vfprintf(fp, message, argptr);
	va_end(argptr);

	fprintf(stderr, "[%s]: ", buf);
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);


	fclose(fp);
	return 1;
}

void logger_log_gl_params() {
	GLenum params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO
	};
	const char *names[] =  {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO"
	};


	logger_log("GL Context Params:\n");
	for(int i = 0; i < 10; ++i) {
		GLint v = 0;
		glGetIntegerv(params[i], &v);
		printf("%d\n",v);
		logger_log("%s %i\n", names[i], v);
	}
	GLint v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	logger_log("%s %i %i\n", names[10], v[0], v[1]);

	GLboolean s = 0;
	glGetBooleanv(params[11], &s);
	logger_log("%s %u\n", names[11], s);
}
