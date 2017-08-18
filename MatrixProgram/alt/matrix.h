#ifndef MATRIX_H
#define MATRIX_H
#define G matrix_G 

#ifndef MATRIX_MULT_BOUNDARY
#define MATRIX_MULT_BOUNDARY 100
#endif

#ifndef MATRIX_TYPE
#define MATRIX_TYPE int 
#endif

#ifndef MATRIX_DESC
#define MATRIX_DESC unsigned long 
#endif

#ifndef MATRIX_DESC_FMT_STR
#define MATRIX_DESC_FMT_STR "%d"
#endif

#ifndef MATRIX_TYPE_FMT_STR
#define MATRIX_TYPE_FMT_STR "%d"
#endif

#define INT MATRIX_TYPE
#define UINT MATRIX_DESC

struct G;

typedef struct G *G;

G matrix_new(UINT row, UINT col);
G matrix_rand(UINT row, UINT col);
G matrix_mult(G a, G b);
G matrix_dot(G a, G b);
G matrix_inv(G a);
G matrix_pinv(G a);
G matrix_map(G a, INT (*)(INT));
G matrix_copy(G a);
G matrix_ident(UINT sz);
G matrix_fill(UINT row, UINT col, INT val);
G matrix_null(UINT row, UINT col);

INT matrix_diagonal(G a);
INT matrix_get(G a, UINT row, UINT col);
INT matrix_det(G a);
INT matrix_eq(G a, G b);

void matrix_set(G a, UINT row, UINT col, INT val);
void matrix_delete(G matrix);
void matrix_print(G matrix);

void matrix_test();



#undef G
#endif  //MATRIX_H
