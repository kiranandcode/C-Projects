#include "functions.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

//             closure    |-----> list of (typename, size, value)
struct variable {
	void *value;
	const char *typename;
	size_t size;
};

static int stringcmp(const void *v_strA, const void *v_strB) {
	const char *strA = v_strA;
	const char *strB = v_strB;

	return strcmp(strA, strB);
}

static unsigned stringhash(const void *v_str) {
	const char *str = v_str;

	unsigned res = 1;

	while(*str) {
		res = (((unsigned )*str * 23) % 17) * res;
		str++;
	}

	return res;
}



function_F function_new(struct F_args (*lambda)(struct F_args closure, struct F_args params), struct F_args closure) {
	function_F func;
	func.lambda = lambda;
	func.closure = closure;
	return func;
}

void function_delete(function_F function) {
	function_argsdelete(&function.closure);	
}

static struct F_args compose(struct F_args closure, struct F_args params) {
	function_F function_a, function_b;
	get(closure, function_a, function_F);
	get(closure, function_b, function_F);
	return function_apply(function_b, function_apply(function_a, params));
}

function_F function_compose(function_F functionA, function_F functionB) {
	function_F result;
	result.lambda = compose;
	result.closure = function_args(2, var(function_F, function_a, functionA),var(function_F, function_b, functionB));
	return result;
}
struct F_args function_apply(function_F function, struct F_args params) {
	return function.lambda(function.closure, params);
}

struct F_args function_args(int count, ...) {
	struct F_args result;
	if(count == 0 || count < 0) {
		result.mapping = NULL;
		return result;
	}
	result.mapping = table_new(count, stringcmp, stringhash);
	va_list ap;
	va_start(ap, count);
	int i;
	for(i = 0; i < count; ++i) {
		char *name = va_arg(ap, char *);
		char *type = va_arg(ap, char *);
		size_t size = va_arg(ap, size_t);
		void *val = va_arg(ap, void *);
		struct variable *var;
		var = malloc(sizeof (*var));
		assert(var);
		var->value = malloc(size);
		memcpy(var->value,val, size);
		var->typename = type;
		var->size = size;
		table_put(result.mapping, name, var);
	}
	va_end(ap);
	return result;
}
void *function_argsget(struct F_args args, char *name, char *type) {
	assert(args.mapping);
	assert(name);
	assert(type);
	struct variable *var;
	var = table_get(args.mapping, name);
	assert(strcmp(var->typename, type) == 0);
	if(strcmp(var->typename, type) != 0) assert(0);

	return var->value;
}

static void variable_delete(const void *key, void **var, void *cl) {
	struct variable *variable = *var;
	free(variable->value);
	free(variable);
	*var = NULL;
}

void function_argsdelete(struct F_args *args) {
	assert(args);
	table_map(args->mapping, variable_delete, NULL);
	table_delete(&args->mapping, NULL);
	args->mapping = NULL;
}
