#ifndef MATRIX_H
#define MATRIX_H
#define G matrix_G 

#ifndef MATRIX_MULT_BOUNDARY
#define MATRIX_MULT_BOUNDARY 100
#endif

#ifndef MATRIX_TYPE
#define MATRIX_TYPE double
#endif

#ifndef MATRIX_DESC
#define MATRIX_DESC unsigned long 
#endif

#ifndef MATRIX_DESC_FMT_STR
#define MATRIX_DESC_FMT_STR "%d"
#endif

#ifndef MATRIX_TYPE_FMT_STR
#define MATRIX_TYPE_FMT_STR "%f"
#endif

#define INT MATRIX_TYPE
#define UINT MATRIX_DESC

struct G;

typedef struct G *G;

G matrix_new(UINT row, UINT col);                  // implemented
G matrix_rand(UINT row, UINT col);                 // implemented
G matrix_mult(G a, G b); 
G matrix_add(G a, G b);                            // implemented
G matrix_sub(G a, G b);                            // implemented
G matrix_dot(G a, G b);                            // implemented
// -----------------------------------------------------------
G matrix_scalar_mult(G a, INT val);                // implemented
G matrix_scalar_add(G a, INT val);                 // implemented
G matrix_scalar_sub(G a, INT val);  	           // implemented
// ----------------------------------------------------------
void matrix_row_swap(G mat, UINT a, UINT b);       // implemented
void matrix_col_swap(G mat, UINT a, UINT b);       // implemented
// -----------------------------------------------------------
G matrix_inv(G a);                                 // implemented
G matrix_pinv(G a);
G matrix_map(G a, INT (*)(INT));                   // implemented
G matrix_copy(G a);                                // implemented
G matrix_ident(UINT sz);                           // implemented
G matrix_fill(UINT row, UINT col, INT val);        // implemented
G matrix_null(UINT row, UINT col);                 // implemented
// -----------------------------------------------------------
G matrix_row(G a, UINT row);                       // implemented
G matrix_col(G a, UINT col);                       // implemented
// -----------------------------------------------------------
G matrix_vertical_concat(G a, G b);                // implemented
G matrix_horizontal_concat(G a, G b);              // implemented
G matrix_transpose(G a);		           // implemented
// -----------------------------------------------------------
INT matrix_diagonal(G a);                          // implemented
INT matrix_get(G a, UINT row, UINT col);           // implemented
INT matrix_det(G a);                               // implemented
INT matrix_eq(G a, G b);                           // implemented
// -----------------------------------------------------------
void matrix_set(G a, UINT row, UINT col, INT val); // implemented
void matrix_delete(G matrix);                      // implemented
void matrix_print(G matrix);                       // implemented
// -----------------------------------------------------------
void matrix_test();                             



#undef G
#endif  //MATRIX_H
