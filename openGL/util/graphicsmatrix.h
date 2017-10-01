#ifndef GRAPHICSMATRIX_H
#define GRAPHICSMATRIX_H
#include "matrix.h"
#define G matrix_G


extern int g_win_width;
extern int g_win_height;

extern int g_fb_width;
extern int g_fb_height;



G graphicsmatrix_cross(G matrixA, G matrixB);

G graphicsmatrix_vec2(INT x, INT y);
G graphicsmatrix_vec3(INT x, INT y, INT z);
G graphicsmatrix_vec4(INT x, INT y, INT z, INT w);

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
