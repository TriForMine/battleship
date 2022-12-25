#include "random.h"

unsigned int generate_random_seed(void) {
    unsigned int seed;

#ifdef _WIN32
    /* Initialize the Windows Cryptographic API (CAPI). */
    HCRYPTPROV hProvider = 0;
    if (!CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        fprintf(stderr, "Error: CryptAcquireContext failed.\n");
        exit(1);
    }

    /* Generate a random seed using CryptGenRandom. */
    if (!CryptGenRandom(hProvider, sizeof(seed), (BYTE*)&seed)) {
        fprintf(stderr, "Error: CryptGenRandom failed.\n");
        exit(1);
    }

    /* Release the CAPI context. */
    CryptReleaseContext(hProvider, 0);
#else
    /* Open /dev/random. */
    int fd = open("/dev/random", O_RDONLY);
    if (fd < 0) {
        perror("Error: open");
        exit(1);
    }

    /* Read a random seed from /dev/random. */
    if (read(fd, &seed, sizeof(seed)) != sizeof(seed)) {
        perror("Error: read");
        close(fd);
    }

    close(fd);
#endif
    return seed;
}