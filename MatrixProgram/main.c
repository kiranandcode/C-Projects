#include "matrix.h"
#include <stdio.h>
#include <assert.h>



int main() {
//	matrix_test();

	matrix_G iden = matrix_ident(3);
	matrix_G rand = matrix_rand(5,5);
	matrix_G random = matrix_horizontal_concat(iden,iden);
	matrix_G vert = matrix_vertical_concat(iden,iden);
	matrix_G row = matrix_row(iden, 1);
	matrix_G col = matrix_col(iden,1);
	matrix_G tr_iden = matrix_transpose(iden);
	matrix_G tr_col = matrix_transpose(col);
	matrix_G tr_rand = matrix_transpose(rand);
	matrix_G next = matrix_inv(iden);


//	matrix_print(col);
//	matrix_print(tr_iden);
//	matrix_print(tr_col);
//	matrix_print(vert);
//	matrix_print(row);
//	matrix_print(random);
//	matrix_print(iden);
//	matrix_print(rand);


	FILE *fp = fopen("./file.mat", "w+");
	
	matrix_serialize(fp, tr_rand);

	fclose(fp);
	fp = fopen("file.mat", "r");

	matrix_G deser_tr_rand = matrix_deserialize(fp);

	fclose(fp);
	assert(
	matrix_eq(deser_tr_rand, tr_rand));
	matrix_print(tr_rand);
	matrix_print(deser_tr_rand);


}
