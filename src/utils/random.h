#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

/* Generate a random seed using /dev/random on Unix-like systems or CryptGenRandom on Windows. */
unsigned int generate_random_seed(void);

#endif /* RANDOM_H */