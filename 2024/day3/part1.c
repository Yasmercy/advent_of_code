#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "vector.h"
#define diff(x, y) (((x) > (y)) ? ((x) - (y)) : ((y) - (x)))

/**
 * the hardcoded DFA for mul(%d,%d)
 * m -> u -> l -> ( -> %d <-> %d -> , -> %d <-> %d -> )
 * states: s ---> m u l ( da1 [da2] [da3] , db1 [db2] [db3] )
 */
vector *parse_line(char *line, size_t size) {
    vector *vec = vector_create(0);
    char lhs[4] = {0};
    char rhs[4] = {0};

    int state = 0;  // 0 to 12
    char *iter = line;
    for (size_t i = 0; i < size; ++iter && ++i) {
        if (state == 0 && *iter == 'm')
            state = 1;
        else if (state == 1 && *iter == 'u')
            state = 2;
        else if (state == 2 && *iter == 'l')
            state = 3;
        else if (state == 3 && *iter == '(')
            state = 4;
        else if (state == 4 && isdigit(*iter))
            state = 5;
        else if (state == 5 && isdigit(*iter))
            state = 6;
        else if (state == 6 && isdigit(*iter))
            state = 7;
        else if (state == 5 && *iter == ',')
            state = 8;
        else if (state == 6 && *iter == ',')
            state = 8;
        else if (state == 7 && *iter == ',')
            state = 8;
        else if (state == 8 && isdigit(*iter))
            state = 9;
        else if (state == 9 && isdigit(*iter))
            state = 10;
        else if (state == 10 && isdigit(*iter))
            state = 11;
        else if (state == 9 && *iter == ')')
            state = 12;
        else if (state == 10 && *iter == ')')
            state = 12;
        else if (state == 11 && *iter == ')')
            state = 12;
        else if (state == 12 && *iter == 'm')
            state = 1;
        else
            state = 0;

        if (state == 5)
            lhs[0] = *iter;
        else if (state == 6)
            lhs[1] = *iter;
        else if (state == 7)
            lhs[2] = *iter;
        else if (state == 9)
            rhs[0] = *iter;
        else if (state == 10)
            rhs[1] = *iter;
        else if (state == 11)
            rhs[2] = *iter;

        if (state == 12) {
            int args[2];
            args[0] = atoi(lhs);
            args[1] = atoi(rhs);
            void *ele;
            memcpy(&ele, args, sizeof(ele));
            vector_push(vec, ele);
        }

        if (state == 0 || state == 12) {
            memset(lhs, 0, 4);
            memset(rhs, 0, 4);
        }
    }

    return vec;
}

int main(void) {
    FILE *f = fopen("data.in", "r");
    char *line = NULL;
    size_t size = 0;

    size_t sum = 0;
    while ((size = getline(&line, &size, f)) != (size_t)-1) {
        vector *parsed = parse_line(line, size);

        for (size_t i = 0; i < parsed->size; ++i) {
            int args[2];
            void *ele = vector_get(parsed, i);
            memcpy(args, &ele, sizeof(args));
            sum += args[0] * args[1];
        }
    }

    printf("sum is %zu\n", sum);
}
