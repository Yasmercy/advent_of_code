#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "vector.h"

int is_valid(size_t sum, size_t cur, vector *ops, size_t start, size_t end)
{
    if (start == end)
        return sum == cur;
    if (end - start == 1)
        return (sum == cur + (size_t)ops->head[start]) || (sum == cur * (size_t)ops->head[start]);

    size_t cur1 = cur + (size_t)ops->head[start];
    size_t cur2 = cur * (size_t)ops->head[start];
    return is_valid(sum, cur1, ops, start + 1, end) || is_valid(sum, cur2, ops, start + 1, end);
}

size_t test_line(char *line)
{
    char *sep = strchr(line, ':');
    *sep = '\0';

    size_t sum = strtoll(line, NULL, 10);
    vector *ops = vector_split_ints(sep + 2, ' ');

    if (is_valid(sum, (size_t)ops->head[0], ops, 1, ops->size))
        return sum;
    return 0;
}

int main(void)
{
    FILE *f = fopen("data.in", "r");

    size_t sum = 0;

    char *line = NULL;
    size_t size;
    while ((size = getline(&line, &size, f)) != (size_t)-1)
    {
        if (line[size - 1] == '\n')
            line[size - 1] = '\0';
        sum += test_line(line);
    }

    printf("sum is %zu\n", sum);
}
