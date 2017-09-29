#include "shadermanager.h"
#include "list.h"

#include "shaderloader.h"
#include "errorchk.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define G shadermanager_G

struct G {
  list_L *collections;
  list_L shader_reference;
  unsigned int size;
};

struct shader_handle {
	shader_G shader;
	char *name;
	GLuint type;
};

G shadermanager_new(unsigned int size) {
	assert(size);
	G shadermanager;
	shadermanager = malloc(sizeof(*shadermanager));
	assert(shadermanager);
	shadermanager->collections = malloc(sizeof(*shadermanager->collections) * size);
	assert(shadermanager->collections);
	int i;
	for(i = 0; (unsigned) i < size; ++i) {
		shadermanager->collections[i] = list_new();
	}	
	shadermanager->shader_reference = list_new();
	shadermanager->size = size;
	return shadermanager;
}

static void *shader_exists(G manager, char *name) {
	assert(manager);
	assert(name);
	struct L_iterator iter = list_iterator(manager->shader_reference);
	while(list_iteratorhasnext(&iter)) {
		struct shader_handle *h = list_iteratornext(&iter);
		assert(h);
		if(!strcmp(h->name, name))
			return h;
	}
	return NULL;
}

static void *shader_existscollection(G manager, int collection, char *name) {
	assert(manager);
	assert(name);
	assert(collection >= 0 && collection < manager->size);
	struct L_iterator iter = list_iterator(manager->collections[collection]);
	while(list_iteratorhasnext(&iter)) {
		struct shader_handle *h = list_iteratornext(&iter);
		assert(h);
		if(!strcmp(h->name, name))
			return h;
	}
	return NULL;
}

void shadermanager_add(G manager, int collection, char *name, GLuint type) {
	assert(0 <= collection && collection  < manager->size);
	assert(manager);
	assert(name);
	struct shader_handle *handle = NULL;
	if((handle = shader_exists(manager, name)) != NULL) {
		if(shader_existscollection(manager, collection, name))
			return;
		else {
			list_append(manager->collections[collection], handle);
		}
	} else {
		// hasn't been added before
		
	       const char *code = loadshader(name);	
	       assert(code != NULL);
	       shader_G shader = shader_new(type, code);

		handle = malloc(sizeof(*handle));
		assert(handle);
		handle->type = type;
		handle->name = malloc(sizeof (*handle->name) * strlen(name));
		assert(handle->name);
		strcpy(handle->name, name);
		handle->shader = shader;
		list_append(manager->shader_reference, handle);
		list_append(manager->collections[collection], handle);
	}
}
GLuint shadermanager_createprogram(G manager, int collection) {
	assert(manager);
	assert(collection >= 0 && collection < manager->size);

	GLuint program = glCreateProgram();

	struct L_iterator iter = list_iterator(manager->collections[collection]);
	while(list_iteratorhasnext(&iter)) {
		struct shader_handle *handle = list_iteratornext(&iter);
		assert(handle);
		if(handle->shader == NULL) {
			handle->shader = shader_new(handle->type, loadshader(handle->name));
		}
		glAttachShader(program, shader_get(handle->shader));
		shader_delete(handle->shader);
		handle->shader = NULL;
	}
	glLinkProgram(program);
	errorchk_program(program);

	return program;

}
void shadermanager_remove(G manager, int collection, char *name) {
	assert(manager);
	assert(collection >= 0 && collection < manager->size);
	assert(name);
	struct shader_handle *handle = NULL;
	if((handle = shader_existscollection(manager, collection, name)) != NULL) {
		list_remove(manager->collections[collection], handle);
		// free up the space once removed
		if(handle->shader != NULL) {
			shader_delete(handle->shader);
			handle->shader = NULL;
		}
	}
}

void shadermanager_removeall(G manager, int collection) {
	assert(manager);
	assert(collection >= 0 && collection < manager->size);

	struct L_iterator iter = list_iterator(manager->collections[collection]);
	while(list_iteratorhasnext(&iter)) {
		struct shader_handle *handle = list_iteratornext(&iter);
		assert(handle);
		if(handle->shader != NULL) {
			shader_delete(handle->shader);
			handle->shader = NULL;
		}
	}
	list_delete(manager->collections[collection], NULL);
	manager->collections[collection] = list_new();
}

void shadermanager_reload(G manager, int collection) {
	assert(manager);
	assert(collection >= 0 && collection < manager->size);

	struct L_iterator iter = list_iterator(manager->collections[collection]);
	while(list_iteratorhasnext(&iter)) {
		struct shader_handle *handle = list_iteratornext(&iter);
		assert(handle);
		if(handle->shader != NULL) {
			shader_delete(handle->shader);
			handle->shader = NULL;
		}
		handle->shader = shader_new(handle->type, loadshader(handle->name));
	}
	
}
void shadermanager_reloadall(G manager) {
	assert(manager);

	struct L_iterator iter = list_iterator(manager->shader_reference);
	
	while(list_iteratorhasnext(&iter)) {
		struct shader_handle *handle = list_iteratornext(&iter);
		assert(handle);
		if(handle->shader != NULL) {
			shader_delete(handle->shader);
			handle->shader = NULL;
		}
		handle->shader = shader_new(handle->type, loadshader(handle->name));
	}
	
}

void handle_delete(void *_handle) {
	struct shader_handle *handle = _handle;
	free(handle->name);
	if(handle->shader != NULL)
		shader_delete(handle->shader);
	free(handle);
}
shader_G shadermanager_get(G manager, char *name) {
	struct shader_handle *handle;
	if((handle = shader_exists(manager, name)) != NULL) {
		if(handle->shader == NULL)
			handle->shader = shader_new(handle->type, loadshader(handle->name));
		return handle->shader;
	} else return NULL;
}
void shadermanager_delete(G manager) {
	assert(manager);

	list_delete(manager->shader_reference, handle_delete);

	int i;
	for(i = 0; (unsigned int)i < manager->size; ++i) {
		list_delete(manager->collections[i], NULL);
	}
	free(manager->collections);
	free(manager);
}


