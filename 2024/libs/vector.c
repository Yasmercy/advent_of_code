#include <assert.h>
#include <string.h>

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

int vector_contains(vector *vec, void *ele)
{
    for (size_t i = 0; i < vec->size; ++i)
    {
        if (vector_get(vec, i) == ele)
            return 1;
    }
    return 0;
}

vector *string_split(char *line, char sep)
{
    vector *vec = vector_create(0);
    char *next = NULL;
    while ((next = strchr(line, sep)) != NULL)
    {
        *next = '\0';
        vector_push(vec, line);
        line = next + 1;
    }

    vector_push(vec, line);
    return vec;
}

vector *vector_split_ints(char *line, char sep)
{
    vector *vec = vector_create(0);
    char *next = NULL;
    while ((next = strchr(line, sep)) != NULL)
    {
        *next = '\0';
        vector_push(vec, (void *)(size_t)atoi(line));
        line = next + 1;
    }

    vector_push(vec, (void *)(size_t)atoi(line));
    return vec;
}
