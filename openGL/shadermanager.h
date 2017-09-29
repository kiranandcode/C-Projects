#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#define G shadermanager_G
struct G;
typedef struct G *G;


struct shadermanager_attriblist_G;
typedef struct shadermanager_attriblist_G *shadermanager_attriblist_G;

shadermanager_attriblist_G shadermanager_attriblistnew(int count, ...);
void shadermanager_attriblistdel(shadermanager_attriblist_G attriblist);

G shadermanager_new(unsigned int size);
void shadermanager_add(G manager, int collection, char *name, GLuint type);
GLuint shadermanager_createprogram(G manager, int collection);
GLuint shadermanager_createattribprogram(G manager, int collection, shadermanager_attriblist_G list);
void shadermanager_remove(G manager, int collection, char *name);
void shadermanager_removeall(G manager, int collection);
void shadermanager_reload(G manager, int collection);
void shadermanager_reloadall(G manager);
shader_G shadermanager_get(G manager, char *name);
void shadermanager_delete(G manager);

#undef G

#endif
