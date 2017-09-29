#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#define G shadermanager_G
struct G;
typedef struct G *G;

G shadermanager_new(unsigned int size);
void shadermanager_add(G manager, int collection, char *name, GLuint type);
GLuint shadermanager_createprogram(G manager, int collection);
void shadermanager_remove(G manager, int collection, char *name);
void shadermanager_removeall(G manager, int collection);
void shadermanager_reload(G manager, int collection);
void shadermanager_reloadall(G manager);
shader_G shadermanager_get(G manager, char *name);
void shadermanager_delete(G manager);

#undef G

#endif
