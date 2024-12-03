#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "vector.h"
#define diff(x, y) (((x) > (y)) ? ((x) - (y)) : ((y) - (x)))

vector *parse_line_to_ints(char *line)
{
    vector *vec = vector_create(0);
    char *iter;
    while ((iter = strchr(line, ' ')) != NULL)
    {
        *iter = '\0';
        vector_push(vec, (void *)(size_t)atoi(line));
        line = iter + 1;
    }
    vector_push(vec, (void *)(size_t)atoi(line));
    return vec;
}

// vector of vectors
vector *read_data(char *filename)
{
    FILE *f = fopen(filename, "r");
    assert(f);

    vector *ret = vector_create(0);

    char *line = NULL;
    size_t size = 0;

    while ((size = getline(&line, &size, f)) != (size_t)-1)
        vector_push(ret, parse_line_to_ints(line));
    fclose(f);

    return ret;
}

int is_all_increasing(vector *vec)
{
    for (size_t i = 0; i < vec->size - 1; ++i)
        if (vec->head[i + 1] <= vec->head[i])
            return 0;
    return 1;
}

int is_all_decreasing(vector *vec)
{
    for (size_t i = 0; i < vec->size - 1; ++i)
        if (vec->head[i + 1] >= vec->head[i])
            return 0;
    return 1;
}

int is_adjacent_close(vector *vec)
{
    for (size_t i = 0; i < vec->size - 1; ++i)
        if (diff(vec->head[i + 1], vec->head[i]) > 3)
            return 0;
    return 1;
}

int main(void)
{
    vector *data = read_data("data.in");

    int count = 0;
    for (size_t i = 0; i < data->size; ++i)
    {
        vector *row = vector_get(data, i);
        int safe = (is_all_increasing(row) || is_all_decreasing(row)) && is_adjacent_close(row);
        count += safe;
    }

    printf("we have %d safe records\n", count);

    for (size_t i = 0; i < data->size; ++i)
        vector_destroy(vector_get(data, i));
    vector_destroy(data);
}
