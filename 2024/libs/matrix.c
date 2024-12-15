#include <assert.h>

#include "../includes/matrix.h"

matrix *matrix_create(size_t rows, size_t cols)
{
    matrix *ret = malloc(sizeof(matrix));
    ret->rows = rows;
    ret->cols = cols;
    ret->data = vector_create(rows * cols);
    return ret;
}

void matrix_destroy(matrix *matrix)
{
    vector_destroy(matrix->data);
    free(matrix);
}

void *matrix_get(matrix *matrix, size_t r, size_t c)
{
    assert(r < matrix->rows && c < matrix->cols);
    return vector_get(matrix->data, r * matrix->cols + c);
}

void matrix_set(matrix *matrix, size_t r, size_t c, void *ele)
{
    assert(r < matrix->rows && c < matrix->cols);
    vector_set(matrix->data, r * matrix->cols + c, ele);
}

matrix *int_matrix_mult(matrix *lhs, matrix *rhs)
{
    assert(lhs->cols == rhs->rows);
    matrix *ret = matrix_create(lhs->rows, rhs->cols);

    for (size_t i = 0; i < lhs->rows; ++i)
    {
        for (size_t j = 0; j < rhs->cols; ++j)
        {
            size_t sum = 0;
            for (size_t k = 0; k < lhs->cols; ++k)
                sum += (size_t)matrix_get(lhs, i, k) * (size_t)matrix_get(rhs, k, j);
            matrix_set(ret, i, j, (void *)sum);
        }
    }

    return ret;
}

// double reinterpret_double_to(void *x)
// {
//     return *(double *)&x;
// }
// 
// void *reinterpret_double_from(double x)
// {
//     return *(void **)&x;
// }
// 
// matrix *double_matrix_mult(matrix *lhs, matrix *rhs)
// {
//     assert(lhs->cols == rhs->rows);
//     matrix *ret = matrix_create(lhs->rows, rhs->cols);
// 
//     for (size_t i = 0; i < lhs->rows; ++i)
//     {
//         for (size_t j = 0; j < rhs->cols; ++j)
//         {
//             double sum = 0;
//             for (size_t k = 0; k < lhs->cols; ++k)
//                 sum += reinterpret_double_to(matrix_get(lhs, i, k)) * reinterpret_double_to(matrix_get(rhs, k, j));
//             matrix_set(ret, i, j, reinterpret_double_from(sum));
//         }
//     }
// 
//     return ret;
// }
