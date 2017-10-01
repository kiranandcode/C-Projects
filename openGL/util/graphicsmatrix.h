#ifndef GRAPHICSMATRIX_H
#define GRAPHICSMATRIX_H
#include "matrix.h"
#define G matrix_G


extern int g_win_width;
extern int g_win_height;

extern int g_fb_width;
extern int g_fb_height;


G graphicsmatrix_mousetoworld(GLint mouse_x, GLint mouse_y, G projection_matrix_inv, G view_matrix_inv);
G graphicsmatrix_cross(G matrixA, G matrixB);

G graphicsmatrix_vec2(INT x, INT y);
G graphicsmatrix_vec3(INT x, INT y, INT z);
G graphicsmatrix_vec4(INT x, INT y, INT z, INT w);
G graphicsmatrix_normalize(G matrix);

G graphicsmatrix_quaternion(INT theta, INT x, INT y, INT z);
G graphicsmatrix_quaternionfromvec(INT theta, G matrix);
int graphicsmatrix_quaternioncheck(G quaternion);
G graphicsmatrix_quaternionnormalize(G quaternion);
G graphicsmatrix_quaternionmult(G quaternion_q, G quaternion_r);
G graphicsmatrix_quaterniontomatrix(G quaternion);
G graphicsmatrix_quaternionslerp(G quaternion_q, G quaternion_r, INT time);

void graphicsmatrix_uniformv(GLuint location, G matrix);
G graphicsmatrix_translation(INT tx, INT ty, INT tz);
G graphicsmatrix_scale(INT sx, INT sy, INT sz);

G graphicsmatrix_rotationX(INT theta);
G graphicsmatrix_rotationY(INT theta);
G graphicsmatrix_rotationZ(INT theta);

G graphicsmatrix_view(G camera_position, G target_position, G up_direction);
G graphicsmatrix_birdseye();
G graphicsmatrix_projection(INT aspect, INT near, INT far, INT fov);


#undef G
#endif // GRAPHICSMATRIX_H
