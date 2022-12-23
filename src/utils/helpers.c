#include "types.h"

Coordinate createCoordinate(unsigned int x, unsigned int y) {
    Coordinate coord;
    coord.x = x;
    coord.y = y;
    return coord;
}

void clearConsole(void) {
#if defined(_WIN32) || defined(_WIN64)
    /* Windows */
    system("cls");
#else
    /* Linux, Mac */
    system("clear");
#endif
}