#ifndef LOGGER_H
#define LOGGER_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>

int logger_restart();
int logger_log(const char *message, ...);
int logger_err(const char *message, ...);
void logger_log_gl_params();

#endif //LOGGER_H
