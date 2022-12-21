#include "main.h"

int main(int argc, char* argv[]) {
    /* Check if a file was specified as an argument */
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            printf("Usage: battleship [file]\n");
            printf("If a file is specified, the game will be loaded from that file.\n");
            printf("If no file is specified, the game will be loaded from stdin.\n");
            return 0;
        } else if (strcmp(argv[1], "-i") == 0) {
            printf("Battleship v1.0\n");
            printf("Written by: Quentin Nicolini and Samy Ben dhiab\n");

        } else {
            parseFile(argv[1]);
        }
    } else {
        parseSTDIN();
    }

    return 0;
}