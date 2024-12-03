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

/* returns 0 for good, -1 for bad, and positive for the position to remove */
int is_approx_all_increasing(vector *vec)
{
    int error_pos = 0;
    int error_count = 0;
    for (size_t i = 0; i < vec->size - 1 && error_count < 2; ++i)
    {
        if (vec->head[i + 1] <= vec->head[i])
        {
            error_pos = i + 1;
            ++error_count;
        }
    }

    return error_count < 2 ? error_pos : -1;
}

/* returns 0 for good, -1 for bad, and positive for the position to remove */
int is_approx_all_decreasing(vector *vec)
{
    int error_pos = 0;
    int error_count = 0;
    for (size_t i = 0; i < vec->size - 1 && error_count < 2; ++i)
    {
        if (vec->head[i + 1] >= vec->head[i])
        {
            error_pos = i + 1;
            ++error_count;
        }
    }

    return error_count < 2 ? error_pos : -1;
}

/* returns 0 for good, -1 for bad, and positive for the position to remove */
int is_approx_adjacent_close(vector *vec)
{
    int error_pos = 0;
    int error_count = 0;
    for (size_t i = 0; i < vec->size - 1 && error_count < 2; ++i)
    {
        if (diff(vec->head[i + 1], vec->head[i]) > 3)
        {
            error_pos = i + 1;
            error_count = error_count + 1 + (i < vec->size - 2 && diff(vec->head[i + 2], vec->head[i]) > 3);
        }
    }

    return error_count < 2 ? error_pos : -1;
}

int main(void)
{
    vector *data = read_data("data.in");

    int count = 0;
    for (size_t i = 0; i < data->size; ++i)
    {
        vector *row = vector_get(data, i);
        int inc = is_approx_all_increasing(row);
        int dec = is_approx_all_decreasing(row);
        int cls = is_approx_adjacent_close(row);

        int safe = (inc != -1 && cls != -1 && (inc == cls || inc * cls == 0)) ||
                   (dec != -1 && cls != -1 && (dec == cls || dec * cls == 0));
        count += safe;
    }

    printf("we have %d safe records\n", count);

    for (size_t i = 0; i < data->size; ++i)
        vector_destroy(vector_get(data, i));
    vector_destroy(data);
}
