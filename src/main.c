#include "main.h"

int main(int argc, char* argv[]) {
    int i;
    bool help = false;
    bool interactive = false;
    char* filename = NULL;

    /* Skip argv[0] (program name). */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help = true;
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interactive") == 0) {
            interactive = true;
        } else {
            filename = argv[i];
        }
    }

    if (help) {
        printf("Usage: battleship [-h] [-i] [file]");
    } else if (interactive) {
        printf("Battleship v1.0\n");
        printf("Written by: Quentin Nicolini and Samy Ben dhiab\n");
    } else if (filename != NULL) {
        parseFile(filename);
    } else {
        parseSTDIN();
    }

    return 0;
}