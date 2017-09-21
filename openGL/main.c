#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

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

	// use this window
	glfwMakeContextCurrent(window);

	// start glew
	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported: %s\n", version);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	glfwTerminate();
	return 0;
}
