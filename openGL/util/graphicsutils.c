#include "graphicsutils.h"

#include "error/logger.h"

#include <stdio.h>
#include <stdlib.h>

int g_win_width = 640;
int g_win_height = 480;

int g_fb_width = 640;
int g_fb_height = 480;

double prev_seconds = 0;
int frame_count = 0;
void update_fps_counter(GLFWwindow *window) {
	double current_seconds;
	double elapsed_seconds;
	current_seconds = glfwGetTime();
	elapsed_seconds = current_seconds - prev_seconds;
	if(elapsed_seconds > 0.25) {
		prev_seconds = current_seconds;
		char tmp[128];
		double fps = (double) frame_count / elapsed_seconds;
		sprintf(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;

}

void glPrintData(FILE *fp, int ref, char *str) {
	fprintf(fp, "%s: %s\n",str, glGetString(ref));
}


void glfw_error_callback(int error, const char *description) {
	logger_err("GLFW ERROR[%i] -> %s\n", error, description);
}

void glfw_window_size_callback(GLFWwindow *window, int width, int height) {
	g_win_width = width;
	g_win_height = height;
}

void glfw_framebuffer_resize_callback(GLFWwindow *window, int width, int height) {
	g_fb_width = width;
	g_fb_height = height;
}

void glfw_require_opengl32() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void glfw_enable_extras() {
	glfwWindowHint(GLFW_SAMPLES, 4);
}

void handleExitPress(GLFWwindow *window) {
	if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window,1);
	}	
}

GLFWwindow *createWindow(int fullscreen) {
	GLFWwindow *window = NULL;
	if(fullscreen) {
		GLFWmonitor* mon = glfwGetPrimaryMonitor();
		const GLFWvidmode *vmode = glfwGetVideoMode(mon);
		window = glfwCreateWindow(vmode->width, vmode->height, "Extended GL Init", mon, NULL);
	} else {
		window = glfwCreateWindow(640,490, "Hello Triangle", NULL, NULL);
	}
	if(!window || window == NULL) {
		logger_err("GLFWERR - Could not open a window\n");
		glfwTerminate();
		return NULL;
	}
	return window;
}



