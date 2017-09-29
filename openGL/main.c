// make sure to include glew  before glfw
// it's like a big header file
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>


#include "shader.h"
#include "shaderloader.h"


void glPrintData(FILE *fp, int ref, char *str) {
	fprintf(fp, "%s: %s\n",str, glGetString(ref));
}



int main() {
	// initialize glfw
	if(!glfwInit()) {
		fprintf(stderr, "ERROR: Could not start GLFW3\n");
		return 1;
	}

	// use glfw to create a window
	GLFWwindow *window = glfwCreateWindow(640, 490, "Hello Triangle", NULL, NULL);

	if(!window) {
		fprintf(stderr, "ERROR: Could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	// use this window - make opengl context use the window
	glfwMakeContextCurrent(window);

	// start glew
	glewExperimental = GL_TRUE;
	glewInit();

	glPrintData(stdout, GL_RENDERER, "Renderer");
	glPrintData(stdout, GL_VERSION, "OpenGL version supported");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	
	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	const char *code = loadshader("test.frag");

	shader_G vs = shader_new(GL_FRAGMENT_SHADER, code);
	code = loadshader("test.vert"); 
	shader_G fs = shader_new(GL_VERTEX_SHADER, code);

	GLuint shader_programme = glCreateProgram();
	
	glAttachShader(shader_programme, shader_get(fs));
	glAttachShader(shader_programme, shader_get(vs));
	glLinkProgram(shader_programme);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}
