#include "stdprof.h"
#include <stdlib.h>

void* malloc_prof(size_t taille) {
    void* p = malloc(taille);
    return p;
}

void* calloc_prof(size_t nb, size_t taille) {
    void* p = calloc(nb, taille);
    return p;
}

void* realloc_prof(void* ptr, size_t taille) {
    void* p = realloc(ptr, taille);
    return p;
}

void free_prof(void* ptr) { free(ptr); }