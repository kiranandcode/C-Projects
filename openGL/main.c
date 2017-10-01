// make sure to include glew  before glfw
// it's like a big header file
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "graphics/shader.h"
#include "graphics/shadermanager.h"
#include "error/logger.h"
#include "error/errorchk.h"

#include "util/graphicsmatrix.h"

static int g_win_width = 640;
static int g_win_height = 480;

static int g_fb_width = 640;
static int g_fb_height = 480;

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

int main() {
	// initialize glfw
	
	glfw_require_opengl32();
	glfw_enable_extras();
	logger_log("Starting GLFW %s\n", glfwGetVersionString());
	glfwSetErrorCallback(glfw_error_callback);

	if(!glfwInit()) {logger_err("ERROR: Could not start GLFW3\n");return 1;}

	// use glfw to create a window
	GLFWwindow *window = createWindow(0);
	if(window == NULL) return 1;

	glfwSetWindowSizeCallback(window, glfw_window_size_callback);
	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_resize_callback);



	// use this window - make opengl context use the window
	glfwMakeContextCurrent(window);

	// start glew
	glewExperimental = GL_TRUE;
	glewInit();


	logger_log_gl_params();
	glPrintData(stdout, GL_RENDERER, "Renderer");
	glPrintData(stdout, GL_VERSION, "OpenGL version supported");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLfloat colours[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

/*	GLfloat matrix[] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.0f, 0.0f, 1.0f
	};*/
	
	matrix_G view_matrix = graphicsmatrix_translation(-0.5f, 0.0f, 0.0f);
	matrix_G rotation_matrix = graphicsmatrix_rotationY(0.01);

	float speed = 2.0f;
	float last_position = 0.0f;

	GLuint vbo[] = {0,0};
	GLuint vao[] = {0};

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);


	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);



	shadermanager_G manager = shadermanager_new(3);
	shadermanager_add(manager, 0, "shaders/test3.frag", GL_FRAGMENT_SHADER);
	shadermanager_add(manager, 0, "shaders/test4.vert", GL_VERTEX_SHADER);

	shadermanager_attriblist_G attribs = shadermanager_attriblistnew(2, 0, "vertex_position", 1, "vertex_colour");


	GLuint shader_programme = shadermanager_createattribprogram(manager, 0, attribs);
	
	int matrix_location = glGetUniformLocation(shader_programme, "matrix");
	glUseProgram(shader_programme);
	glUseProgram(shader_programme);
		//glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);
		graphicsmatrix_uniformv(matrix_location, view_matrix);
	glUseProgram(0);

	logger_log_program(shader_programme);


	// enable back face culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	double prev_seconds = glfwGetTime();
	while(!glfwWindowShouldClose(window)) {
		double current_seconds = glfwGetTime();

		double elapsed_seconds = current_seconds - prev_seconds;
		prev_seconds = current_seconds;
		if(fabs(last_position) > 1.0f) {
			speed = -speed;
		}
		matrix_set(view_matrix, 0, 3, elapsed_seconds * speed + last_position);
		last_position = matrix_get(view_matrix, 0, 3);


		matrix_G new_matrix = matrix_mult(view_matrix, rotation_matrix);
		matrix_delete(view_matrix);
		view_matrix = new_matrix;

//		matrix[12] = elapsed_seconds * speed + last_position;
//		last_position = matrix[12];
		glUseProgram(shader_programme);
			graphicsmatrix_uniformv(matrix_location, view_matrix);
//			glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);
		glUseProgram(0);


		update_fps_counter(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.6f,0.6f, 0.8f, 1.0f);
		glViewport(0, 0, g_fb_width, g_fb_height);

		glUseProgram(shader_programme);
			glBindVertexArray(vao[0]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		glUseProgram(0);


		glfwPollEvents();
		glfwSwapBuffers(window);


		if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R)) {
			shadermanager_reloadall(manager);
			glDeleteProgram(shader_programme);

			shader_programme = shadermanager_createattribprogram(manager, 0, attribs);


		}

		handleExitPress(window);
	}


	glfwTerminate();
	return 0;
}
