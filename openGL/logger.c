#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"

const char *GL_type_to_string(GLenum type) {
	switch(type) {
		case GL_BOOL:
			return "bool";
		case GL_INT:
			return "int";
		case GL_FLOAT:
			return "float";
		case GL_FLOAT_VEC2:
			return "vec2";
		case GL_FLOAT_VEC3:
			return "vec3";
		case GL_FLOAT_VEC4:
			return "vec4";
		case GL_FLOAT_MAT2:
			return "mat2";
		case GL_FLOAT_MAT3:
			return "mat3";
		case GL_FLOAT_MAT4:
			return "mat4";
		case GL_SAMPLER_2D:
			return "sampler2D";
		case GL_SAMPLER_3D:
			return "sampler3D";
		case GL_SAMPLER_CUBE:
			return "samplerCube";
		case GL_SAMPLER_2D_SHADOW:
			return "sampler2DShadow";
		default:
			break;
	}
	return "other";
}

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

void logger_log_program(GLuint program) {
	logger_log("------------------------------\nshader programme %i info:\n", program);

	
	int params = -1;
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	logger_log("GL_LINK_STATUS = %i\n", params);

	glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
	logger_log("GL_ATTACHED_SHADERS = %i\n", params);

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
	logger_log("GL_ACTIVE_ATTRIBUTES = %i\n", params);
	
	GLuint i;
	for(i = 0; i < (GLuint)params; ++i) {
		int max_length = 64;
		int actual_length = 0;
		char name[max_length];
		int size;

		GLenum type;
		glGetActiveAttrib(program, i, max_length, &actual_length, &size, &type, name);
		if(size > 1) {
			int j;
			for(j = 0; j < size; j++) {
				char long_name[max_length];
				sprintf(long_name, "%s[%i]", name, j);
				int location = glGetAttribLocation(program, long_name);
				logger_log("   %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), long_name, location);
			}
		} else {
			int location = glGetAttribLocation(program, name);
			logger_log("   %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), name, location);
		}

	}

	glGetProgramiv(program,GL_ACTIVE_UNIFORMS, &params);
	logger_log("GL_ACTIVE_UNIFORMS = %i\n", params);

	for(i = 0; i < (GLuint) params; ++i) {

		int max_length = 64;
		int actual_length = 0;
		char name[max_length];
		int size;
		GLenum type;
		glGetActiveUniform(program, i, max_length, &actual_length, &size, &type, name);
		if(size > 1) {
			int j;
			for(j = 0; j  < size; j++) {
				char long_name[max_length];
				sprintf(long_name, "%s[%i]", name,j);
				int location = glGetUniformLocation(program, long_name);
				printf("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string(type), long_name, location);
			}
		}
	}

}


