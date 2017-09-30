#ifndef GRAPHICSMATRIX_H
#define GRAPHICSMATRIX_H
#include "matrix.h"
#define G matrix_G

void graphicsmatrix_uniformv(GLuint location, G matrix);
G graphicsmatrix_translation(INT tx, INT ty, INT tz);
G graphicsmatrix_scale(INT sx, INT sy, INT sz);

G graphicsmatrix_rotationX(INT theta);
G graphicsmatrix_rotationY(INT theta);
G graphicsmatrix_rotationZ(INT theta);

G graphicsmatrix_view(G rotationvector, G forwardvector, G upvector, G positionvector);
G graphicsmatrix_birdseye();
G graphicsmatrix_projection(INT aspect, INT near, INT far, INT fov);

#undef G
#endif // GRAPHICSMATRIX_H
