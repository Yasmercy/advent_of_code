/* fixed size matrix implementation */

#include "vector.h"

typedef struct matrix
{
    size_t rows;
    size_t cols;
    vector *data;
} matrix;

matrix *matrix_create(size_t rows, size_t cols);

void matrix_destroy(matrix *matrix);

void *matrix_get(matrix *matrix, size_t r, size_t c);

void matrix_set(matrix *matrix, size_t r, size_t c, void *ele);

matrix *int_matrix_mult(matrix *lhs, matrix *rhs);

matrix *double_matrix_mult(matrix *lhs, matrix *rhs);
