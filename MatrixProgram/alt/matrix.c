#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <time.h>
#include <stdlib.h>

#define G matrix_G

struct matrix_G {
	UINT row;
	UINT col;
	INT *val;
};

int srand_completed = 0;


void init_rand() {
	if(srand_completed) return;

	srand(time(NULL));

	srand_completed = 1;
	return;
}

G matrix_new(UINT row, UINT col) {
	//assert(row > 0 && col > 0);

	G matrix = malloc(sizeof(*(matrix)));

	assert(matrix);

	matrix->row = row;
	matrix->col = col;

	UINT sz = row * col;

	matrix->val = calloc(sz, sizeof(UINT));

	assert(matrix->val);

	return matrix;
}

G matrix_rand(UINT row, UINT col) {
	init_rand();
	G matrix = matrix_new(row,col);

	UINT i,j;

	for(j = 0; j < matrix->row; ++j){
		for(i = 0; i <matrix->col; ++i) {
			*(matrix->val + (j*matrix->col) + i) = rand();
		}
	}

	return matrix;
}

void matrix_mult_normal(G a, G b, G c) {
	assert(a->col == b->row);
	assert(c->row == a->row && c->col == b->col);
	UINT i,j,k;

	for(j = 0; j < c->row; ++j){
		for(i = 0; i < c->col; ++i) {
			INT val = 0;

			for(k = 0; k < a->col; ++k){
				val += *(a->val + (j * a->col) + k) * *(b->val + (k * b->col) + i);
			}

			*(c->val + (j*c->col) + i) = val;

		}


	}

}

G matrix_mult(G a, G b) {
	assert(a->col == b->row);

	G matrix = matrix_new(a->row, b->col);

	if(a->row > MATRIX_MULT_BOUNDARY &&
	   a->col > MATRIX_MULT_BOUNDARY &&
	   b->col > MATRIX_MULT_BOUNDARY &&
	   b->row > MATRIX_MULT_BOUNDARY) {

		// strassens multiplication


	} else {
		// normal multiplication
		matrix_mult_normal(a,b,matrix);	
	}

	return matrix;
}

G matrix_dot(G a, G b) {
	assert(a->row == b->row && a->col == b->col);
	UINT i,j;
	G matrix = matrix_new(a->row, a->col);

	for(j = 0; j<a->row; ++j) {
		for(i = 0; i <a->col; ++i){
			*(matrix->val + (matrix->col * j) + i) = *(a->val + (a->col * j) + i) * *(b->val + (b->col * j) + i);
		}
	}

	return matrix;
}

G matrix_map(G a, INT (*f)(INT)){
	assert(a && f);

	G matrix = matrix_new(a->row, a->col);

	UINT i,j;

	for(j = 0; j < matrix->row; ++j){
		for(i = 0; i < matrix->col; ++i) {
	*(matrix->val + (j*matrix->col) + i) = f(*(a->val + (j*a->col) + i));
		}
	}


	return matrix;
}

G matrix_copy(G a) {
	G matrix = matrix_new(a->row, a->col);
	assert(matrix);
	
	UINT i,j;

	for(j = 0; j < a->row; ++j) {
		for(i = 0; i < a->col; ++i) {
	*(matrix->val + (j*matrix->col) + i) = *(a->val + (j*a->col) + i); 
		}
	}
	return matrix;
}

G matrix_ident(UINT sz) {
	assert(sz > 0);

	G matrix = matrix_new(sz,sz);

	UINT j;

	for(j = 0; j < matrix->row; ++j)
		*(matrix->val + (j * matrix->col) + j) = 1;

	return matrix;
}

G matrix_fill(UINT row, UINT col, INT val) {
	//assert(row > 0 && col > 0);

	G matrix = matrix_new(row,col);
	assert(matrix);

	UINT i,j;


	for(j = 0; j < matrix->row; ++j) {
		for(i = 0; i < matrix->col; ++i) {
	*(matrix->val + (j * matrix->col) + i) = val;
		}
	}

	return matrix;
}

G matrix_null(UINT row, UINT col) {
	return matrix_fill(row,col, 0);
}

INT matrix_diagonal(G a) {
	assert(a->row == a->col);

	INT val = 1;

	UINT k;

	for(k = 0; k < a->col; ++k) {
		val *= *(a->val + (k * a->col) + k);
	}

	return val;
}


INT matrix_get(G a, UINT row, UINT col) {
	assert(a->col > col && a->row > row);

	return *(a->val + (row * a->col) + col);
}

INT matrix_det(G a) {
	assert(a->row == a->col);
	return 1;
}

INT matrix_eq(G a, G b) {
	UINT i,j;

	if(a->row != b->row || a->col != b->col)
		return 0;

	for(j = 0; j<a->row;++j){
		for(i = 0; i <a->col; ++i){

			if(*(a->val + (j*a->col) + i) != *(b->val + (j*b->col) + i)) return 0;
		}
	}

	return 1;
}

void matrix_set(G a, UINT row, UINT col, INT val) {
	assert(a->col > col && a->row > row);

	*(a->val + (row * a->col) + col) = val;

}

void matrix_delete(G matrix) {
	assert(matrix);
	assert(matrix->val);

	free(matrix->val);
	free(matrix);

	return;
}


void matrix_print(G matrix) {
	UINT i,j;

	printf("[");
	for(j = 0; j < matrix->row; ++j) {
		for(i = 0; i < matrix->col; i++) {
			printf(MATRIX_TYPE_FMT_STR, *(matrix->val + (j * matrix->col) + i));
			if(i != matrix->col-1)
				printf(", ");
		}
		if(j == matrix->row - 1)
			printf("]\n");
		else
			printf("\n");
	}
}



void matrix_test() {
	init_rand();

	printf("Running unit tests...\n");
	int test_count = 8;

	// test matrix identity 
	printf("Test (1/%d): Testing Identity generator\n",test_count);
	G matrix_test = matrix_ident(rand()%100);
	UINT i,j;

	for(j = 0; j <matrix_test->row; ++j){
		for(i = 0; i < matrix_test->col; ++i) {

			if(i == j) {
				assert(*(matrix_test->val + (j * matrix_test->col) + i) == 1);
			} else {
				assert(*(matrix_test->val + (j * matrix_test->col) + i) == 0);
			}

		}
	}
	matrix_delete(matrix_test);

	printf("\tTest Passed\n");

	// test matrix diagonal
	printf("Test (2/%d): Testing Diagonal of Identity is 1\n",test_count);

	matrix_test = matrix_ident(rand()%100);
	INT val = matrix_diagonal(matrix_test);

	assert(val == 1);

	matrix_delete(matrix_test);

	printf("\tTest Passed\n");


	// test matrix copy
	printf("Test (3/%d): Testing copied matrix has equal rows and columns\n", test_count);

	matrix_test = matrix_rand(rand()%100, rand()%100);

	G matrix_clone = matrix_copy(matrix_test);

	assert(matrix_clone->row == matrix_test->row && matrix_clone->col == matrix_test->col);

	matrix_delete(matrix_test);
	matrix_delete(matrix_clone);

	printf("\tTest Passed\n");

	
	printf("Test (4/%d): Testing copied matrix has equal fields\n", test_count);

	matrix_test = matrix_rand(rand()%100, rand()%100);

	matrix_clone = matrix_copy(matrix_test);

	for(j = 0; j<matrix_test->row;++j){
		for(i = 0; i < matrix_test->col; ++i){
			assert(*(matrix_test->val + (j*matrix_test->col) + i) == *(matrix_clone->val + (j*matrix_clone->col) + i));
		}
	}

	

	matrix_delete(matrix_test);
	matrix_delete(matrix_clone);

	printf("\tTest Passed\n");


	printf("Test (5/%d): Testing equality function\n", test_count);
	matrix_test = matrix_rand(rand()%100, rand()%100);
	matrix_clone = matrix_copy(matrix_test);

	assert(matrix_eq(matrix_test, matrix_clone));

	matrix_delete(matrix_test);
	matrix_delete(matrix_clone);

	printf("\tTest Passed\n");

	printf("Test (6/%d): Testing Matrix multiplication of identity is the same\n", test_count);
	UINT sz = rand()%100;

	matrix_test = matrix_rand(sz,sz);
	matrix_clone = matrix_ident(sz);

	G matrix_result = matrix_mult(matrix_test, matrix_clone);

	assert(matrix_eq(matrix_result, matrix_test));

	matrix_delete(matrix_test);
	matrix_delete(matrix_clone);
	matrix_delete(matrix_result);

	printf("\tTest Passed\n");


	printf("Test (7/%d): Testing null matrix is null\n", test_count);
	matrix_test = matrix_null(rand()%100, rand()%100);

	


	for(j = 0; j <matrix_test->row; ++j){
		for(i = 0; i < matrix_test->col; ++i) {
				assert(*(matrix_test->val + (j * matrix_test->col) + i) == 0);
		}
	}

	matrix_delete(matrix_test);
	printf("\tTest Passed\n");


	printf("Test (8/%d): Testing fill matrix is filled\n", test_count);

	sz = rand()%100;
	matrix_test = matrix_fill(rand()%100, rand()%100, sz);

	for(j = 0; j <matrix_test->row; ++j){
		for(i = 0; i < matrix_test->col; ++i) {
				assert(*(matrix_test->val + (j * matrix_test->col) + i) == sz);
		}
	}

	matrix_delete(matrix_test);
	printf("\tTest Passed\n");



}
