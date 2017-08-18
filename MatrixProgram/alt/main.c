#include "matrix.h"



int main() {
	matrix_test();

	matrix_G iden = matrix_ident(3);
	matrix_G rand = matrix_rand(5,10);

	matrix_print(iden);
	matrix_print(rand);

}
