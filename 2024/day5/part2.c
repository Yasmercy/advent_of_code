#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "vector.h"
#define swap(vec, i, j)                                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        void *__tmp = vector_get(vec, i);                                                                              \
        vector_set(vec, i, vector_get(vec, j));                                                                        \
        vector_set(vec, j, __tmp);                                                                                     \
    } while (0);

void add_edge(vector *adj_list, char *line)
{
    char *sep = strchr(line, '|');
    *sep = '\0';

    size_t left = atoi(line);
    size_t right = atoi(sep + 1);

    vector_push(vector_get(adj_list, left), (void *)right);
}

int cmp(vector *adj_list, size_t before, size_t after)
{
    vector *adj;

    adj = vector_get(adj_list, before);
    if (vector_contains(adj, (void *)after))
        return 1;

    adj = vector_get(adj_list, after);
    if (vector_contains(adj, (void *)before))
        return -1;

    return 0;
}

/* returns 0 if invalid */
size_t is_valid(vector *adj_list, vector *nodes)
{
    // check if valid -> no breaking rules
    for (size_t i = 0; i < nodes->size; ++i)
    {
        size_t before = (size_t)vector_get(nodes, i);
        for (size_t j = 0; j < i; ++j)
        {
            size_t after = (size_t)vector_get(nodes, j);
            if (cmp(adj_list, before, after) > 0)
                return 0;
        }
    }
    return 1;
}

size_t median_page(vector *adj_list, vector *nodes)
{
    for (size_t i = 0; i < nodes->size; ++i)
    {
        for (size_t j = 1; j < nodes->size; ++j)
        {
            size_t before = (size_t)vector_get(nodes, j - 1);
            size_t after = (size_t)vector_get(nodes, j);
            if (cmp(adj_list, before, after) > 0)
                swap(nodes, j - 1, j);
        }
    }

    return (size_t)vector_get(nodes, nodes->size / 2);
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
        else
        {
            vector *nodes = vector_split_ints(line, ',');
            if (stage == 1 && !is_valid(adj_list, nodes))
                sum += median_page(adj_list, nodes);
            vector_destroy(nodes);
        }
    }

    printf("sum is %zu\n", sum);
    for (size_t i = 0; i < adj_list->size; ++i)
        vector_destroy(adj_list->head[i]);
    vector_destroy(adj_list);
    fclose(f);
}
