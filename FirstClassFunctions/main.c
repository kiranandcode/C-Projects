#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

// printf \a b -> print(a + b + c)
struct F_args function(struct F_args closure, struct F_args params) {

	int c = get(closure, c, int);
	int a = get(params, a, int);
	int b = get(params, b, int);
	printf("%d + %d + %d = %d\n",a, b, c, c + a + b);
	return function_args(0);
}


// \c -> (a: c +3,b: c*2)
struct F_args function2(struct F_args closure, struct F_args params) {
	int c = get(params, c, int);
	int cp3 = c + 3;
	int cs3 = c * 3;
	return function_args(2, var(int, a, cp3), var(int, b, cs3));
}


int main() {
	int i = 3;
	function_F lambdaB = function_new(function, function_args(1, var(int, c, i)));
	function_F lambdaA = function_new(function2, function_args(0));
	function_apply(lambdaB, function_apply(lambdaA, function_args(1, var(int, c, i))));

	printf("--------Now composing-------\n");

	i = 4;
	function_F lambdaBA = function_compose(lambdaA,lambdaB);
	function_apply(lambdaBA, function_args(1, var(int, c, i)));
	
}
