#include <assert.h>
#include <stdio.h>

#include "vector.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(b, a) (((a) < (b)) ? (a) : (b))
#define abs(x) ((x) < 0) ? (-1 * (x)) : (x)

// vector {*left, *right}
vector **read_data(char *filename)
{
    FILE *f = fopen(filename, "r");
    assert(f);

    vector *left = vector_create(0);
    vector *right = vector_create(0);

    char *line = NULL;
    size_t size = 0;

    size_t l, r;
    while ((size = getline(&line, &size, f)) != (size_t)-1)
    {
        sscanf(line, "%zd   %zd", &l, &r);
        // printf("%zd %zd\n", l, r);
        vector_push(left, (void *)l);
        vector_push(right, (void *)r);
    }
    fclose(f);

    vector **ret = calloc(2, sizeof(vector *));
    ret[0] = left;
    ret[1] = right;
    return ret;
}

size_t count_occurences(vector *vec, void *ele)
{
    size_t count = 0;
    for (size_t i = 0; i < vec->size; ++i)
    {
        void *other = vector_get(vec, i);
        count += ele == other;
    }

    return count;
}

int main(void)
{
    vector **data = read_data("data.in");
    vector *left = data[0];
    vector *right = data[1];

    assert(left->size == right->size);

    size_t sum = 0;
    for (size_t i = 0; i < left->size; ++i)
    {
        void *ele = vector_get(left, i);
        size_t count = count_occurences(right, ele);

        // printf("%zu %zu\n", (size_t)ele, count);

        sum += (size_t)ele * count;
    }

    printf("difference between left and right is %zu\n", sum);

    vector_destroy(left);
    vector_destroy(right);
    free(data);
}
