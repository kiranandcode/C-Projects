//
// Created by gopia on 01/04/2017.
//

#ifndef MATRIXPROGRAM_MATRIX_H
#define MATRIXPROGRAM_MATRIX_H

#include "stdlib.h"
#include "stdio.h"


typedef struct {
    double **matrix;
    const int rows;
    const int columns;
} *matrix, _matrix;


matrix getEmptyMatrix(int rows, int columns);

/**
 * @brief Initializes a matrix from a corresponding mxn matrix.
 * @param rows The number of rows.
 * @param columns The number of columns
 * @param array The double[rows][columns] matrix to use as initialization.
 * @return The mxn matrix object.
 */
matrix getInitializedMatrix(int rows, int columns, double** array);


/**
 * @brief A function to prdouble a matrix as with any good abstraction.
 * @param mat The matrix to prdouble.
 */
void printMatrix(matrix mat);


/**
 * @brief Function to get a specific value in a matrix.
 * Intended for use by end user, library specific functions do not use this function, but directly access the matrix.
 * Also performs bounds checking, so don't exceed the bounds, or the program will terminate.
 * @param i The row number - following mathematical notation, the first row is numbered 1 rather than the programming standard of 0.
 * @param j The column number - following mathematical notation, the first column is numbered 1 rather than the programming standard of 0.
 * @param mat The matrix to retrieve values from.
 * @return The value.
 */
double get(int i, int j, matrix mat);


/**
 * @brief Function returning the identity matrix of order n.
 * @param order The order of the matrix, the number of rows and columns it should have.
 * @return The identity matrix.
 */
matrix getIdentityMatrix(int order);


/**
 * @brief A function to return the diagonal product of a square matrix.
 * @param mat The matrix to perform the calculation on.
 * @return The diagonal product.
 */
double diagonalProduct(matrix mat);

/**
 * @brief A function to create a copy of a specified matrix.
 * @param mat A matrix to copy.
 * @return A copy of the matrix.
 */
matrix copyMatrix(matrix mat);

/**
 * @brief A function to find the determinant of a matrix (using gaussian elimination).
 * @param mat A square matrix.
 * @return The determinant of said matrix.
 */
double matrixDeterminant(matrix mat);


#endif //MATRIXPROGRAM_MATRIX_H
