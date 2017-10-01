#include "matrix.h"
#include "matrixrep.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <time.h>
#include <stdlib.h>

#define G matrix_G

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

G matrix_csv_fload(FILE *fp, UINT row, UINT col) {

	G matrix = matrix_new(row,col);
	UINT i;
	//char buf[10];
	//sprintf(buf, "%s,", MATRIX_TYPE_FMT_STR);

	for(i = 0; i < row*col; ++i) {
		fscanf(fp, MATRIX_TYPE_FMT_STR, matrix->val+i);
		char c = getc(fp);

		//assert((c == ',' || c == EOF || c == '\0' || c == '\n'));
	
		if(!(c == ',' || c == EOF || c == '\0' || c == '\n'))
			ungetc(c,fp);
	}
	return matrix;
}



G matrix_load(UINT row, UINT col, INT *values){
	G matrix = matrix_new(row, col);

	UINT i;

	for(i = 0; i < row * col; i++) {
		matrix->val[i] = values[i];
	}

	return matrix;
}

UINT matrix_row_get(G matrix) {
	return matrix->row;
}

UINT matrix_col_get(G matrix) {
	return matrix->col;
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

		// TODO: strassens multiplication
		matrix_mult_normal(a,b,matrix);	


	} else {
		// normal multiplication
		matrix_mult_normal(a,b,matrix);	
	}

	return matrix;
}


G matrix_add(G a, G b) {
	assert(a->col == b->col && a->row == b->row);
	G matrix = matrix_new(a->row, a->col);

	UINT i,j;

	for(j = 0; j < a->row; ++j)
		for(i = 0; i < a->col; ++i) {
			*(matrix->val + (matrix->col * j) + i) = *(a->val + (a->col * j) + i) + *(b->val + (b->col * j) + i);  
		}

	return matrix;
}

G matrix_sub(G a, G b) {
	assert(a->col == b->col && a->row == b->row);
	G matrix = matrix_new(a->row, a->col);

	UINT i,j;

	for(j = 0; j < a->row; ++j)
		for(i = 0; i < a->col; ++i) {
			*(matrix->val + (matrix->col * j) + i) = *(a->val + (a->col * j) + i) - *(b->val + (b->col * j) + i);  
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

INT matrix_dotproduct(G a, G b) {
	assert(a->row == b->row && a->col == b->col);
	UINT i,j;
	INT sum = 0;

	for(j = 0; j<a->row; ++j) {
		for(i = 0; i <a->col; ++i){
			sum += *(a->val + (a->col * j) + i) * *(b->val + (b->col * j) + i);
		}
	}

	return sum;
}

G matrix_normalize(G a) {
	assert(a);
	G matrix = matrix_new(a->row, a->col);

	UINT i,j;
	INT sum = 0;

	for(j = 0; j < a->row; ++j)
		for(i = 0; i < a->col; ++i) {
			sum += (*(a->val + (a->col * j) + i) * *(a->val + (a->col * j) + i)); 
		}
	if(!((sum < 0 && sum < -0.0001) || (sum > 0 && sum > 0.0001)))
		sum = 1;
	for(j = 0; j < a->row; ++j)
		for(i = 0; i < a->col; ++i) {
			*(matrix->val + (matrix->col * j) + i) = *(a->val + (a->col * j) + i) / sum; 
		}



	return matrix;
}




G matrix_scalar_mult(G a, INT val) {

	assert(a);
	G matrix = matrix_copy(a);
	UINT j,i;

	for(j = 0; j < a->row; ++j)
		for(i = 0; i < a->col; ++i) {
			*(matrix->val + (matrix->col * j) + i) *= val;
		}
	return matrix;
}

G matrix_scalar_add(G a, INT val) {

	assert(a);
	G matrix = matrix_copy(a);
	UINT j,i;

	for(j = 0; j < a->row; ++j)
		for(i = 0; i < a->col; ++i) {
			*(matrix->val + (matrix->col * j) + i) += val;
		}
	return matrix;
}

G matrix_scalar_sub(G a, INT val) {

	assert(a);
	G matrix = matrix_copy(a);
	UINT j,i;

	for(j = 0; j < a->row; ++j)
		for(i = 0; i < a->col; ++i) {
			*(matrix->val + (matrix->col * j) + i) -= val;
		}
	return matrix;
}

void matrix_row_swap(G mat, UINT a, UINT b){

	assert(a < mat->row && b < mat->row);

	if(a == b) return;

	INT temp;
	UINT i;

	for(i = 0; i < mat->col; ++i) {
		temp = *(mat->val + (a*mat->col) + i);
		*(mat->val + (a*mat->col) + i) = *(mat->val + (b*mat->col) + i);
		*(mat->val + (b*mat->col) + i) = temp;
	}
}

void matrix_col_swap(G mat, UINT a, UINT b){

	assert(a < mat->col && b < mat->col);

	if(a == b) return;

	INT temp;
	UINT i;

	for(i = 0; i < mat->row; ++i) {
		temp = *(mat->val + (i*mat->col) + a);
		*(mat->val + (i*mat->col) + a) = *(mat->val + (i*mat->col) + b);
		*(mat->val + (i*mat->col) + b) = temp;
	}
}

G matrix_inv(G a) {
	assert(a);
	assert(a->row == a->col);

	G row_echelon = matrix_copy(a);
	G inverse = matrix_ident(a->row);

	UINT i,j,k, i_k;
	INT ratio;

	// check for 0 values in column
	assert(matrix_diagonal(a) != 0);

	
	// if 1st row is 0, move next row in place
	if(*(row_echelon->val) == 0)
		for(i = 0; i < row_echelon->row; i++)
			if(*(row_echelon->val + (i*row_echelon->col)) != 0){
				matrix_row_swap(row_echelon, 0, i);	
				matrix_row_swap(inverse, 0, i);
				break;
			}

		


	for(i = 0; i < a->row-1; ++i) {
		for(j = i+1; j < a->row; ++j){


	if(*(row_echelon->val + (j*row_echelon->col) + i) == 0)
		for(i_k = j; i_k < row_echelon->row; i_k++)
			if(*(row_echelon->val + (i_k*row_echelon->col)) != 0){
				matrix_row_swap(row_echelon, j, i_k);	
				matrix_row_swap(inverse, j, i_k);
				break;
			}



ratio = *(row_echelon->val + (j*row_echelon->col) + i) / *(row_echelon->val + (i*row_echelon->col) + i);
			for(k = 0; k < a->col; ++k) {
*(row_echelon->val + (j*row_echelon->col) + k) -= *(row_echelon->val + (i*row_echelon->col) + k) * ratio;
*(inverse->val + (j*inverse->col) + k) -= *(inverse->val + (i*inverse->col) + k) * ratio;
			}
		
		}
	}
		
	
	for(i = a->row-1; i > 0; --i){
		for(j = 0; j < i; ++j){
			ratio = *(row_echelon->val + (j*row_echelon->col) + i) / *(row_echelon->val + (i*row_echelon->col) + i);


			for(k = 0; k < a->col; ++k) {
*(row_echelon->val + (j*row_echelon->col) + k) -= *(row_echelon->val + (i*row_echelon->col) + k) * ratio;
*(inverse->val + (j*inverse->col) + k) -= *(inverse->val + (i*inverse->col) + k) * ratio;
			}
		}
	}

	
	for(i = 0; i < a->row; ++i) {
		if (*(row_echelon->val + (i*row_echelon->col) + i) != 1) {
			INT temp = *(row_echelon->val + (i*row_echelon->col) + i);

			if(temp == 0) {
				matrix_delete(row_echelon);
				matrix_delete(inverse);
				return NULL;
			}
			for(k = 0; k < a->col; ++k) {
				*(row_echelon->val + (i*row_echelon->col) + k) = *(row_echelon->val + (i*row_echelon->col) + k) / temp;
				*(inverse->val + (i*inverse->col) + k) = *(inverse->val + (i*inverse->col) + k) / temp;
			}
		}
	}

	
	matrix_delete(row_echelon);
	return inverse;
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

G matrix_row(G a, UINT row) {
	assert(row < a->row);
	G matrix = matrix_new(1, a->col);

	UINT i;
	for(i = 0; i < a->col; ++i) {
		*(matrix->val + i) = *(a->val + (row*a->col) + i);
	}

	return matrix;
}

G matrix_col(G a, UINT col) {
	assert(col < a->col);
	G matrix = matrix_new(a->row, 1);

	UINT i;
	for(i = 0; i < a->row; ++i) {
*(matrix->val + (i*matrix->col)) = 
*(a->val + (i*a->col) + col);
	}

	return matrix;
}

G matrix_vertical_concat(G a, G b) {
	assert(a->col == b->col);

	G matrix = matrix_new(a->row + b->row, a->col);

	UINT i,j;

	// copy over the matrix a
	for(j = 0; j < a->row; ++j){
		for(i = 0; i < a->col; ++i){
	*(matrix->val + (j*a->col) + i) = *(a->val + (j*a->col) + i);
		}
	}
	
	for(j = 0; j < b->row; ++j){
		for(i = 0; i < b->col; ++i){
	*(matrix->val + ((j + a->row)*a->col) + i) = *(b->val + (j*b->col) + i);
		}
	}

	return matrix;
}

G matrix_horizontal_concat(G a, G b) {
	assert(a->row == b->row);

	G matrix = matrix_new(a->row, a->col + b->col);

	UINT i,j;

	// copy over the matrix a
	for(j = 0; j < a->row; ++j){
		for(i = 0; i < a->col; ++i){
	*(matrix->val + (j*a->col) + i) = *(a->val + (j*a->col) + i);
		}
	}
	
	for(j = 0; j < b->row; ++j){
		for(i = 0; i < b->col; ++i){
	*(matrix->val + (j * (a->col + b->col)) + i + (a->col)) = *(b->val + (j*b->col) + i);
		}
	}

	return matrix;
}

	G matrix_transpose(G a) {
		assert(a);

		G matrix = matrix_new(a->col, a->row);
		UINT i,j;

		for(j = 0; j < a->col; ++j){
			for(i = 0; i < a->row; ++i) {
		*(matrix->val + (matrix->col * j) + i) = *(a->val + (a->col * i) + j);
			}
		}


		return matrix;
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
		G upper_triangular = matrix_copy(a);
		G lower_triangular = matrix_ident(a->row);

		UINT i,j,k;
		INT ratio;
		INT det;

		// check for 0 values in column
		assert(matrix_diagonal(a) != 0);

		for(i = 0; i < a->row-1; ++i) {
			for(j = i+1; j < a->row; ++j){
				ratio = *(upper_triangular->val + (j*upper_triangular->col) + i) / *(upper_triangular->val + (i*upper_triangular->col) + i);
				for(k = 0; k < a->col; ++k) {
		*(upper_triangular->val + (j*upper_triangular->col) + k) -= *(upper_triangular->val + (i*upper_triangular->col) + k) * ratio;
		*(lower_triangular->val + (j*lower_triangular->col) + k) -= *(lower_triangular->val + (i*lower_triangular->col) + k) * ratio;
				}
			}
		}
		

		det = matrix_diagonal(upper_triangular) * matrix_diagonal(lower_triangular);


		matrix_delete(upper_triangular);
		matrix_delete(lower_triangular);
		return det;
	}

	INT matrix_eq(G a, G b) {
		UINT i,j;

		
		if(a->row != b->row || a->col != b->col) {
			return 0;
		}

		for(j = 0; j<a->row;++j){
			for(i = 0; i <a->col; ++i){
				if(*(a->val + (j*a->col) + i) == 0.0) *(a->val + (j*a->col) + i) = 0; 
				if(*(b->val + (j*b->col) + i) == 0.0)	*(b->val + (j*b->col) + i) = 0;

				INT bb = *(b->val + (j*b->col) + i);
				INT aa = *(a->val + (j * a->col) + i);

				if(aa > bb) {
					if(aa - bb > 0.000001){ 
						return 0;}
				} else if(bb > aa)
					if(bb - aa > 0.000001){
						return 0;
					}
			}
		}
		return 1;
	}

	void matrix_set(G a, UINT row, UINT col, INT val) {
		assert(a->col > col && a->row > row);

		*(a->val + (row * a->col) + col) = val;

	}

	G matrix_reshape(G matrix, UINT row, UINT col) {
	assert(matrix);
	assert(row && col);
	G result = matrix_new(row,col);


	UINT i,j;
	UINT jmax = (row < matrix->row ? row : matrix->row), imax = (col < matrix->col ? col : matrix->col);

		for(j = 0; j < jmax; ++j) 
			for(i = 0; i < imax; ++i)
			matrix_set(result, j, i, matrix_get(matrix, j, i));

		return result;
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
				printf(";\n");
		}
	}





UINT matrix_high_value_row(G matrix, UINT row){
	assert(row < matrix->row);

	UINT highest_index = 0;
	INT highest = *(matrix->val + (matrix->col * row));

	UINT i;
	for(i = 0; i < matrix->col; ++i){

		INT val = *(matrix->val + (matrix->col * row) + i);
		if(val > highest) (highest = val), highest_index = i;
	}

	return highest_index;
}

UINT matrix_high_value_col(G matrix, UINT col) {
	assert(col < matrix->col);

	UINT highest_index =0 ;
	INT highest = *(matrix->val + col);

	UINT i;
	for(i = 0; i<matrix->row; ++i) {
		INT val = *(matrix->val + (matrix->col * i) + col);
		if(val > highest) (highest = val), highest_index = i;
	}
	return highest_index;
}



void matrix_serialize(FILE *fp, G matrix) {
	assert(fp != NULL);

	char buf[40];

	sprintf(buf, "matrix[%s][%s]\n", MATRIX_DESC_FMT_STR, MATRIX_DESC_FMT_STR);

	fprintf(fp, buf, matrix->row, matrix->col);

	UINT i;
	for(i = 0; i< matrix->row * matrix->col; ++i) {

		fprintf(fp, MATRIX_TYPE_FMT_STR, *(matrix->val + i));

		if(i != matrix->row * matrix->col -1) {
			fprintf(fp, ",");
		}
	}
}

G matrix_deserialize(FILE *fp) {
	UINT row,col;
	char buf[30];
	sprintf(buf, "matrix[%s][%s]", MATRIX_DESC_FMT_STR, MATRIX_DESC_FMT_STR);
	char c;
	while((c = getc(fp)) != 'm');
	ungetc(c,fp);
	fscanf(fp, buf, &row, &col);
	return matrix_csv_fload(fp, row, col);
}



void matrix_test() {
	init_rand();

	printf("Running unit tests...\n");
	int test_count = 11;

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

	printf("Test (9/%d): Testing add function\n", test_count);

	sz = rand()%100;
	UINT b = rand()%sz;

	UINT row = rand()%10;
	UINT col = rand()%10;

	matrix_test = matrix_fill(row,col, sz);
	matrix_clone = matrix_fill(row,col, b);
	matrix_result = matrix_add(matrix_test, matrix_clone);


	for(j = 0; j <matrix_test->row; ++j){
		for(i = 0; i < matrix_test->col; ++i) {
				assert(*(matrix_result->val + (j * matrix_result->col) + i) == sz + b);
		}
	}

	matrix_delete(matrix_test);
	matrix_delete(matrix_clone);
	matrix_delete(matrix_result);
	printf("\tTest Passed\n");


	printf("Test (10/%d): Testing sub function\n", test_count);

	sz = rand()%100;
	b = rand()%sz;
	
	row = rand()%10;
	col = rand()%10;

	matrix_test = matrix_fill(row, col, sz);
	matrix_clone = matrix_fill(row, col, b);
	matrix_result = matrix_sub(matrix_test, matrix_clone);

	for(j = 0; j <matrix_test->row; ++j){
		for(i = 0; i < matrix_test->col; ++i) {
				assert(*(matrix_result->val + (j * matrix_result->col) + i) == sz - b);
		}
	}

	matrix_delete(matrix_test);
	matrix_delete(matrix_clone);
	matrix_delete(matrix_result);
	printf("\tTest Passed\n");


	printf("Test (11/%d): Testing dot function\n", test_count);

	sz = rand()%100;
	b = rand()%100;

	matrix_test = matrix_fill(sz,b,1);
	matrix_clone = matrix_rand(sz,b);

	matrix_result = matrix_dot(matrix_test, matrix_clone);

	assert(matrix_eq(matrix_result, matrix_clone));

	matrix_delete(matrix_test);
	matrix_delete(matrix_clone);
	matrix_delete(matrix_result);
	printf("\tTest Passed\n");

	printf("Test (12/%d): Testing Transpose of Null is Null\n", test_count);

	sz = rand()%100;
	matrix_test = matrix_null(sz,sz);
	matrix_clone = matrix_transpose(matrix_test);

	assert(matrix_eq(matrix_clone, matrix_test));
	

	matrix_delete(matrix_test);
	matrix_delete(matrix_clone);

	printf("\tTest Passed\n");

	printf("Test (13/%d): Testing multiplying an inverse results in identity\n", test_count);

	sz = 3;
	matrix_test = matrix_rand(sz,sz);
	

	for(j = 0; j < matrix_test->row; ++j) {
		for(i = 0; i < matrix_test->col; ++i){
*(matrix_test->val + (matrix_test->col * j) + i) = ((int)*(matrix_test->val + (matrix_test->col * j) + i) % 10);
		}
	}



	matrix_clone = matrix_inv(matrix_test);

	matrix_result = matrix_mult(matrix_test, matrix_clone);

	matrix_print(matrix_test);
	matrix_print(matrix_clone);
	matrix_print(matrix_result);


	G ident = matrix_ident(sz);

	assert(matrix_eq(ident, matrix_result));
	

	matrix_delete(matrix_test);
	matrix_delete(matrix_clone);
	matrix_delete(matrix_result);
	matrix_delete(ident);

	printf("\tTest Passed\n");


}
