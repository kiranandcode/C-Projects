#include "matrix.h"



int main() {
	matrix_test();

	matrix_G iden = matrix_ident(3);
	matrix_G rand = matrix_rand(5,10);
	matrix_G random = matrix_horizontal_concat(iden,iden);
	matrix_G vert = matrix_vertical_concat(iden,iden);
	matrix_G row = matrix_row(iden, 1);
	matrix_G col = matrix_col(iden,1);

	matrix_print(col);
	matrix_print(vert);
	matrix_print(row);
	matrix_print(random);
	matrix_print(iden);
	matrix_print(rand);

}
