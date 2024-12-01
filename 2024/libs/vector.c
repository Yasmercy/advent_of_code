#include <assert.h>

#include "../includes/vector.h"

size_t round_up_to_power_of_2(size_t size)
{
    return (size < 2) ? 1 : 1 << (8 * sizeof(size_t) - __builtin_clzll(size - 1));
}

vector *vector_create(size_t size)
{
    vector *vec = calloc(1, sizeof(vector));
    vector_resize(vec, size);
    return vec;
}

void vector_destroy(vector *vec)
{
    free(vec->head);
    free(vec);
}

void vector_resize(vector *vec, size_t size)
{
    vec->size = size;
    vec->capacity = round_up_to_power_of_2(size);
    vec->head = realloc(vec->head, vec->capacity * sizeof(void *));
}

void vector_push(vector *vec, void *ele)
{
    vector_resize(vec, vec->size + 1);
    vector_set(vec, vec->size - 1, ele);
}

void *vector_pop(vector *vec)
{
    assert(vec->size > 0);
    void *ret = vector_get(vec, vec->size - 1);
    vector_resize(vec, vec->size - 1);
    return ret;
}

void vector_set(vector *vec, size_t pos, void *ele)
{
    assert(pos < vec->size);
    vec->head[pos] = ele;
}

void *vector_get(vector *vec, size_t pos)
{
    return vec->head[pos];
}
