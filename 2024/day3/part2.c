#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "vector.h"

static int mul_enable = 1;

/**
 * the hardcoded DFA for mul(%d,%d)
 * m -> u -> l -> ( -> %d <-> %d -> , -> %d <-> %d -> )
 * states: s ---> m u l ( da1 [da2] [da3] , db1 [db2] [db3] )
 */
vector *parse_line(char *line, size_t size) {
    vector *vec = vector_create(0);
    char lhs[4] = {0};
    char rhs[4] = {0};

    int mul_state = 0;   // 0 to 12
    int do_state = 0;    // 0 to 3
    int dont_state = 0;  // 0 to 5
    char *iter = line;
    for (size_t i = 0; i < size; ++iter && ++i) {
        if (mul_state == 0 && *iter == 'm')
            mul_state = 1;
        else if (mul_state == 1 && *iter == 'u')
            mul_state = 2;
        else if (mul_state == 2 && *iter == 'l')
            mul_state = 3;
        else if (mul_state == 3 && *iter == '(')
            mul_state = 4;
        else if (mul_state == 4 && isdigit(*iter))
            mul_state = 5;
        else if (mul_state == 5 && isdigit(*iter))
            mul_state = 6;
        else if (mul_state == 6 && isdigit(*iter))
            mul_state = 7;
        else if (mul_state == 5 && *iter == ',')
            mul_state = 8;
        else if (mul_state == 6 && *iter == ',')
            mul_state = 8;
        else if (mul_state == 7 && *iter == ',')
            mul_state = 8;
        else if (mul_state == 8 && isdigit(*iter))
            mul_state = 9;
        else if (mul_state == 9 && isdigit(*iter))
            mul_state = 10;
        else if (mul_state == 10 && isdigit(*iter))
            mul_state = 11;
        else if (mul_state == 9 && *iter == ')')
            mul_state = 12;
        else if (mul_state == 10 && *iter == ')')
            mul_state = 12;
        else if (mul_state == 11 && *iter == ')')
            mul_state = 12;
        else if (mul_state == 12 && *iter == 'm')
            mul_state = 1;
        else
            mul_state = 0;

        if (mul_state == 5)
            lhs[0] = *iter;
        else if (mul_state == 6)
            lhs[1] = *iter;
        else if (mul_state == 7)
            lhs[2] = *iter;
        else if (mul_state == 9)
            rhs[0] = *iter;
        else if (mul_state == 10)
            rhs[1] = *iter;
        else if (mul_state == 11)
            rhs[2] = *iter;

        if (do_state == 0 && *iter == 'd')
            do_state = 1;
        else if (do_state == 1 && *iter == 'o')
            do_state = 2;
        else if (do_state == 2 && *iter == '(')
            do_state = 3;
        else if (do_state == 3 && *iter == ')') {
            do_state = !(mul_enable = 1);
        } else
            do_state = 0;

        if (dont_state == 0 && *iter == 'd')
            dont_state = 1;
        else if (dont_state == 1 && *iter == 'o')
            dont_state = 2;
        else if (dont_state == 2 && *iter == 'n')
            dont_state = 3;
        else if (dont_state == 3 && *iter == '\'')
            dont_state = 4;
        else if (dont_state == 4 && *iter == 't')
            dont_state = 5;
        else if (dont_state == 5 && *iter == '(')
            dont_state = 6;
        else if (dont_state == 6 && *iter == ')') {
            mul_enable = dont_state = 0;
        } else
            dont_state = 0;

        if (mul_state == 12 && mul_enable) {
            int args[2];
            args[0] = atoi(lhs);
            args[1] = atoi(rhs);

            void *ele;
            memcpy(&ele, args, sizeof(ele));
            vector_push(vec, ele);
        }

        if (mul_state == 0 || mul_state == 12) {
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
            void *ele = vector_get(parsed, i);

            int args[2];
            memcpy(args, &ele, sizeof(args));
            sum += args[0] * args[1];
        }
    }

    printf("sum is %zu\n", sum);
}
