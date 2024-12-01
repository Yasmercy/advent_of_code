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

size_t get_argmin(vector *vec)
{
    size_t min_pos = 0;
    void *min_ele = vector_get(vec, min_pos);

    for (size_t i = 1; i < vec->size; ++i)
    {
        void *ele = vector_get(vec, i);
        if (ele < min_ele)
        {
            min_pos = i;
            min_ele = ele;
        }
    }

    return min_pos;
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
        size_t l = get_argmin(left);
        size_t r = get_argmin(right);

        size_t ll = (size_t)vector_get(left, l);
        size_t rr = (size_t)vector_get(right, r);

        sum += (ll < rr) ? (rr - ll) : ll - rr;
        vector_set(left, l, (void *)-1);
        vector_set(right, r, (void *)-1);
    }

    printf("difference between left and right is %zu\n", sum);

    vector_destroy(left);
    vector_destroy(right);
    free(data);
}
