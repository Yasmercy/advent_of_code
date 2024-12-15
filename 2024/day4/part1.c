#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "vector.h"
#define diff(x, y) (((x) > (y)) ? ((x) - (y)) : ((y) - (x)))

// implementing the Z-array algorithm
int main(void) {
    FILE *f = fopen("data.in", "r");
    char *line = NULL;
    size_t size = 0;

    vector *lines = vector_create(0);
    while ((size = getline(&line, &size, f)) != (size_t)-1) {
        line[size - 1] = '\0';
        vector_push(lines, line);
    }

    // do something here
}
