#include "main.h"
#include "multiplayer/socket.h"

int main(int argc, char *argv[]) {
    int i;
    bool help = false;
    bool interactive = false;
    char *filename = NULL;
    long benchmark = 0;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help = true;
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interactive") == 0) {
            interactive = true;
        } else if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) && i + 1 < argc) {
            filename = argv[i + 1];
        } else if ((strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--benchmark") == 0) && i + 1 < argc) {
            benchmark = strtol(argv[i + 1], NULL, 10);
        }
    }

    if (help) {
        printf("Usage: battleship [-h] [-i] [file]");
    } else if (interactive) {
        handleInteractiveGame();
    } else if (filename != NULL) {
        parseFile(filename);
    } else if (benchmark > 0) {
        startBenchmark(benchmark);
    } else {
        parseSTDIN();
    }

    return 0;
}