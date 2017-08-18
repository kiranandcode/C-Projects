//
// Created by gopia on 01/04/2017.
//

#include "matrix.h"


matrix getEmptyMatrix(int rows, int columns){
    // Create the matrix
    matrix mat = malloc(sizeof(_matrix));

    // Initialize the representation
    *(int *)&mat->columns = columns;
    *(int *)&mat->rows = rows;

    // Create the rows
    mat->matrix = malloc(sizeof(double*)*rows);

    // Create each column and set all values to 0.
    int i, j;
    for(i = 0; i<rows; i++) {
        mat->matrix[i] = malloc(sizeof(double)*columns);
        for(j = 0; j<columns; j++) {
            mat->matrix[i][j] = 0;
        }
    };
    return mat;
}


matrix getInitializedMatrix(int rows, int columns, double** array){
    // Create the matrix
    matrix mat = malloc(sizeof(_matrix));

    // Initialize the representation
    *(int *)&mat->columns = columns;
    *(int *)&mat->rows = rows;

    // Create the rows
    mat->matrix = malloc(sizeof(double*)*rows);

    // Create each column and set all values to 0.
    int i, j;
    for(i = 0; i<rows; i++) {

        mat->matrix[i] = malloc(sizeof(double)*columns);
        for(j = 0; j<columns; j++) {
            mat->matrix[i][j] = array[i][j];
        }
    };
    return mat;
}


void printMatrix(matrix mat) {
    int i, j;
    // Iterate through each row, prdouble each value.
    for(i = 0; i<mat->rows; i++) {
        printf("[");
        for(j = 0; j< mat->columns; j++) {
            printf("%.1lf, ", mat->matrix[i][j]);
        }
        printf("\b\b]\n");
    }
}


double get(int i, int j, matrix mat) {
    // Matrix indexes are usually addressed from 1.
    i -= 1;
    j -= 1;
    // Check the bounds
    if(i > mat->rows || i < 0) printf("Error: get Called on invalid parameters. (%d, %d)", i, j), exit(1);
    if(j > mat->columns || j < 0) printf("Error: get Called on invalid parameters. (%d, %d)", i, j), exit(1);

    return mat->matrix[i][j];
}

matrix getIdentityMatrix(int order) {
    // Get 0 Matrix
    matrix mat = getEmptyMatrix(order, order);

    // Set every index where i == j to 0.
    int i;
    for(i = 0; i<order; i++) {
        mat->matrix[i][i] = 1;
    }

    return mat;
}


double diagonalProduct(matrix mat) {
    // Check called on square matrix
    if(mat->columns != mat->rows) printf("Error: diagonalProduct called on non-square matrix."), exit(1);

    // Multiply all the diagonal elements together.
    int i;
    double product = 1;
    for(i = 0; i<mat->columns; i++) {
        product *= mat->matrix[i][i];
    }
    return product;
}


matrix copyMatrix(matrix mat) {
    // Construct an empty matrix.
    matrix newMat = getEmptyMatrix(mat->rows, mat->columns);

    // Update each of the elements of the new to equal the original.
    int i, j;
    for(i = 0; i<mat->rows; i++) {
        for(j = 0; j<mat->columns; j++) {
            newMat->matrix[i][j] = mat->matrix[i][j];
        }
    }

    return newMat;
}


double matrixDeterminant(matrix mat) {
    // Check called on square matrix
    if(mat->columns != mat->rows) printf("Error: diagonalProduct called on non-square matrix."), exit(1);

    // Make copies of the matrix so as not to mutate the input.
    matrix upper_triangular = copyMatrix(mat);
    // The identity matrix will eventually become the lower triangular matrix after finishing all operations
    matrix lower_triangular = getIdentityMatrix(mat->columns);


    // For each diagonal element, eliminate all the values beneath it.

    int i,j,k,l;
    // Iterate through all rows except the last(as there are no values to "eliminate" beneath it.
    for(i = 0; i<upper_triangular->rows-1; i++) {
        // Iterate through all the rows beneath the selected one.
        for(j = i+1; j < upper_triangular->rows; j++) {
            // Calculate the ratio required to eliminate the element for this row that is beneath the diagonal element.
            double ratio = upper_triangular->matrix[j][i]/upper_triangular->matrix[i][i];
            for(k = 0; k<upper_triangular->columns; k++) {
                // Update all of the columns, also apply the same operation to the identity matrix to get the lower_triangular.
                upper_triangular->matrix[j][k] = upper_triangular->matrix[j][k] - ratio * upper_triangular->matrix[i][k];
                lower_triangular->matrix[j][k] = lower_triangular->matrix[j][k] - ratio * lower_triangular->matrix[i][k];
            }
        }
    }
    double determinant = diagonalProduct(upper_triangular)*diagonalProduct(lower_triangular);

    return determinant;
}
