#include "graphicsmatrix.h"
#include "matrixrep.h"

#include <assert.h>
#include <math.h>

#define G matrix_G

G graphicsmatrix_cross(G matrixA, G matrixB) {
	assert(matrixA && matrixB);
	assert(matrixA && matrixA->row == 3 && matrixA->col == 1);
	assert(matrixB && matrixB->row == 3 && matrixB->col == 1);
	G result = matrix_new(3, 1);

	INT componentA = (matrixA->val[1] * matrixB->val[2]) - (matrixA->val[2] * matrixB->val[1]);
	INT componentB = (matrixA->val[3] * matrixB->val[0]) - (matrixA->val[0] * matrixB->val[3]);
	INT componentC = (matrixA->val[0] * matrixB->val[1]) - (matrixA->val[1] * matrixB->val[0]);

	matrix_set(result, 0,0, componentA);
	matrix_set(result, 1,0, componentB);
	matrix_set(result, 2,0, componentC);

	return result;
}


G graphicsmatrix_vec2(INT x, INT y) {
	G result = matrix_new(2,1);
	matrix_set(result, 0, 0, x);
	matrix_set(result, 1, 0, y);
	return result;
}

G graphicsmatrix_vec3(INT x, INT y, INT z) {
	G result = matrix_new(3,1);
	matrix_set(result, 0, 0, x);
	matrix_set(result, 1, 0, y);
	matrix_set(result, 2, 0, z);
	return result;
}

G graphicsmatrix_vec4(INT x, INT y, INT z, INT w) {
	G result = matrix_new(4, 1);
	matrix_set(result, 0, 0, x);
	matrix_set(result, 1, 0, y);
	matrix_set(result, 2, 0, z);
	matrix_set(result, 3, 0, w);
	return result;
}


void graphicsmatrix_uniformv(GLuint location, G matrix) {
	assert(matrix);
	assert((matrix->row == matrix->col) && (matrix->row == 2 || matrix->row == 3 || matrix->row == 4));
	switch(matrix->row) {
		case 2:
		glUniformMatrix2fv(location, 1, GL_TRUE, matrix->val);
			break;
		case 3:
		glUniformMatrix3fv(location, 1, GL_TRUE, matrix->val);
			break;
		case 4:
		glUniformMatrix4fv(location, 1, GL_TRUE, matrix->val);
			break;
		default:
			assert(0);
	}
}



G graphicsmatrix_translation(INT tx, INT ty, INT tz){ 
	G matrix =  matrix_ident(4);
	matrix_set(matrix, 0, 3, tx);
	matrix_set(matrix, 1, 3, ty);
	matrix_set(matrix, 2, 3, tz);
	return matrix;
}

G graphicsmatrix_scale(INT sx, INT sy, INT sz) {
	G matrix = matrix_ident(4);
	matrix_set(matrix, 0, 0, sx);
	matrix_set(matrix, 1, 1, sy);
	matrix_set(matrix, 2, 2, sz);
	return matrix;
}

G graphicsmatrix_rotationX(INT theta) {
	G matrix = matrix_ident(4);

	matrix_set(matrix, 1, 1, cos(theta));
	matrix_set(matrix, 1, 2, -sin(theta));
	matrix_set(matrix, 2, 1, sin(theta));
	matrix_set(matrix, 2, 2, cos(theta));
	return matrix;
}

G graphicsmatrix_rotationY(INT theta) {
	G matrix = matrix_ident(4);

	matrix_set(matrix, 0, 0, cos(theta));
	matrix_set(matrix, 0, 2, sin(theta));
	matrix_set(matrix, 2, 0, -sin(theta));
	matrix_set(matrix, 2, 2, cos(theta));
	return matrix;
}

G graphicsmatrix_rotationZ(INT theta) {
	G matrix = matrix_ident(4);

	matrix_set(matrix, 0, 0, cos(theta));
	matrix_set(matrix, 0, 1, -sin(theta));
	matrix_set(matrix, 1, 0, sin(theta));
	matrix_set(matrix, 1, 1, cos(theta));
	return matrix;
}

static G graphicsmatrix_view_internal(G rotationvector, G forwardvector, G upvector, G positionvector) {
	assert(rotationvector);
	assert(forwardvector);
	assert(upvector);
	assert(positionvector);

	assert(rotationvector->row == 3 && rotationvector->col == 1);
	assert(forwardvector->row == 3 && forwardvector->col == 1);
	assert(upvector->row == 3 && upvector->col == 1);
	assert(positionvector->row == 3 && positionvector->col == 1);
	G matrix = matrix_ident(4);

	matrix_set(matrix, 0, 3, -positionvector->val[0]);
	matrix_set(matrix, 1, 3, -positionvector->val[1]);
	matrix_set(matrix, 2, 3, -positionvector->val[2]);

	matrix_set(matrix, 0, 0, rotationvector->val[0]);
	matrix_set(matrix, 0, 1, rotationvector->val[1]);
	matrix_set(matrix, 0, 2, rotationvector->val[2]);

	matrix_set(matrix, 1, 0, upvector->val[0]);
	matrix_set(matrix, 1, 1, upvector->val[1]);
	matrix_set(matrix, 1, 2, upvector->val[2]);

	matrix_set(matrix, 2, 0, forwardvector->val[0]);
	matrix_set(matrix, 2, 1, forwardvector->val[1]);
	matrix_set(matrix, 2, 2, forwardvector->val[2]);

	return matrix;
}

G graphicsmatrix_view(G camera_position, G target_position, G up_direction) {
	assert(camera_position && camera_position->row == 3 && camera_position->col == 1);
	assert(target_position && target_position->row == 3 && target_position->col == 1);
	assert(up_direction && up_direction->row == 3 && up_direction->col == 1);


	G diff = matrix_sub(target_position, camera_position);
	G forward_direction = matrix_normalize(diff);

	G rotation_direction = graphicsmatrix_cross(forward_direction, up_direction);
	G reup_direction = graphicsmatrix_cross(rotation_direction, forward_direction);

	G result = graphicsmatrix_view_internal(rotation_direction, forward_direction, reup_direction, camera_position);
	matrix_delete(diff);
	matrix_delete(forward_direction);
	matrix_delete(rotation_direction);
	matrix_delete(reup_direction);

	return result;
}


G graphicsmatrix_birdseye() {
	G matrix = matrix_new(4,4);
	matrix_set(matrix, 0, 0, 1);
	matrix_set(matrix, 2, 1, -1);
	matrix_set(matrix, 1, 2, 1);
	matrix_set(matrix, 3, 3, 1);
	return matrix;
}

static G graphicsmatrix_projection_internal(INT sx, INT sy, INT sz, INT pz) {
	G matrix = matrix_ident(4);
	
	matrix_set(matrix, 0, 0, sx);
	matrix_set(matrix, 1, 1, sy);
	matrix_set(matrix, 2, 2, sz);
	matrix_set(matrix, 2, 3, pz);
	matrix_set(matrix,3, 2, -1);
	matrix_set(matrix,3, 3,   0);

	return matrix;
}

G graphicsmatrix_projection(INT aspect, INT near, INT far, INT fov) {
	INT range = tan(fov/2)*near;
	INT sx = (2 * near) / (2 * (range*aspect));
	INT sy = near / range;
	INT sz = -(far + near) / (far - near);
	INT pz = -(2 * far * near) / (far - near);

	return graphicsmatrix_projection_internal(sx, sy, sz,pz);
}


