#ifndef STD_PROF_H
#define STD_PROF_H

#include <stddef.h>
#include <stdlib.h>

void* malloc_prof(size_t taille);
void* calloc_prof(size_t nb, size_t taille);
void free_prof(void* ptr);

#endif /* STD_PROF_H */
