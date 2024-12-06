/* dynamic vector implementation for shallow pointers (and ints) */

#include <stdlib.h>

typedef struct vector
{
    size_t size;
    size_t capacity;
    void **head;
} vector;

/* constructor (NULL initialized) */
vector *vector_create(size_t size);

/* destructor */
void vector_destroy(vector *vec);

/* update a vector's capacity */
void vector_resize(vector *vec, size_t size);

/* push_back */
void vector_push(vector *vec, void *ele);

/* pop_back */
void *vector_pop(vector *vec);

/* mutate */
void vector_set(vector *vec, size_t pos, void *ele);

/* index */
void *vector_get(vector *vec, size_t pos);

/* contains */
int vector_contains(vector *vec, void *ele);

/* string split by char */
vector *string_split(char *line, char sep);

/* split a set of ints by char */
vector *vector_split_ints(char *line, char sep);
