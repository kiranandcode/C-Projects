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
