#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"





double main() {
    double **example = malloc(sizeof(double *)*4);
    int i;
    for(i = 0; i<4; i++) {
        example[i] = malloc(sizeof(double)*4);
    }
    example[0][0] = 2.0;
    example[0][1] = -2.0;
    example[0][2] = 1.0;
    example[0][3] = 0.0;

    example[1][0] = 0.0;
    example[1][1] = 1.0;
    example[1][2] = -3.0;
    example[1][3] = 4.0;

    example[2][0] = 2.0;
    example[2][1] = -1.0;
    example[2][2] = -1.0;
    example[2][3] = 4.0;

    example[3][0] = -6.0;
    example[3][1] = 4.0;
    example[3][2] = 3.0;
    example[3][3] = -7.0;


    matrix det = getInitializedMatrix(4, 4, example);
    printMatrix(det);
    printf("Determinant is %lf\n",matrixDeterminant(det));

    return 0;
}