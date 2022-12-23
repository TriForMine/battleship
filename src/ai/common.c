#include "common.h"

void placeRandomShips(Board* board, const int* shipLengths, int shipLengthsLength) {
    Coordinate coordinate;
    bool success = false;
    int i, j;

    for (i = 0; i < shipLengthsLength; i++) {
        do {
            int shipLength = shipLengths[i];
            unsigned int x = rand() % board->WIDTH;
            unsigned int y = rand() % board->HEIGHT;
            int orientation = rand() % 2;
            if (orientation == 0) {
                if (x + shipLength > board->WIDTH) {
                    x -= shipLength;
                }
            } else {
                if (y + shipLength > board->HEIGHT) {
                    y -= shipLength;
                }
            }

            for (j = 0; j < shipLength; j++) {
                if (orientation == HORIZONTAL) {
                    coordinate = createCoordinate(x + j, y);
                } else {
                    coordinate = createCoordinate(x, y + j);
                }

                if (getTileState(board, coordinate) == SHIP || false == isCoordinateValid(board, coordinate)) {
                    success = false;
                    break;
                }
                success = true;
            }

            if (!success) {
                continue;
            }

            placeShip(board, createShip(shipLength, orientation), createCoordinate(x, y));
        } while (success == false);
    }
}
