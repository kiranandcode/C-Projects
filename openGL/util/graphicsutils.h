#ifndef GRAPHICSUTILS_H
#define GRAPHICSUTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

void update_fps_counter(GLFWwindow *window);
void glPrintData(FILE *fp, int ref, char *str);
void glfw_error_callback(int error, const char *description);
void glfw_window_size_callback(GLFWwindow *window, int width, int height);
void glfw_framebuffer_resize_callback(GLFWwindow *window, int width, int height);
void glfw_require_opengl32();
void glfw_enable_extras();
void handleExitPress(GLFWwindow *window);
GLFWwindow *createWindow(int fullscreen);

#endif //GRAPHICSUTILS_H
