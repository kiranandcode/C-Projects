#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>



typedef struct shader_G *shader_G;
shader_G shader_new(GLuint type, const GLchar *code);
void shader_delete(shader_G shader);
GLuint shader_get(shader_G shader);

#endif
