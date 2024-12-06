#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "vector.h"
#define diff(x, y) (((x) > (y)) ? ((x) - (y)) : ((y) - (x)))

void add_edge(vector *adj_list, char *line)
{
    char *sep = strchr(line, '|');
    *sep = '\0';

    size_t left = atoi(line);
    size_t right = atoi(sep + 1);

    vector_push(vector_get(adj_list, left), (void *)right);
}

/* returns 0 if invalid */
size_t median_page(vector *adj_list, char *line)
{
    vector *nodes = vector_split_ints(line, ',');

    // check if valid -> no breaking rules
    for (size_t i = 0; i < nodes->size; ++i)
    {
        size_t before = (size_t)vector_get(nodes, i);
        vector *adj = vector_get(adj_list, before);
        for (size_t j = 0; j < i; ++j)
        {
            size_t after = (size_t)vector_get(nodes, j);
            if (vector_contains(adj, (void *)after))
                return 0;
        }
    }


    // must be valid here
    size_t med = (size_t)vector_get(nodes, nodes->size / 2);
    vector_destroy(nodes);
    return med;
}

int main(void)
{
    FILE *f = fopen("data.in", "r");
    char *line = NULL;
    size_t size = 0;

    // vector of vectors
    vector *adj_list = vector_create(100);
    for (size_t i = 0; i < adj_list->size; ++i)
        adj_list->head[i] = vector_create(0);

    int stage = 0; // 0 for before updates, 1 for during
    size_t sum = 0;
    while ((size = getline(&line, &size, f)) != (size_t)-1)
    {
        line[size - 1] = '\0';
        if (size == 1)
        {
            stage = 1;
            continue;
        }

        if (stage == 0)
            add_edge(adj_list, line);
        if (stage == 1)
            sum += median_page(adj_list, line);
    }

    printf("sum is %zu\n", sum);
}
