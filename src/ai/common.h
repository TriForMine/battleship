#ifndef BATTLESHIP_COMMON_H
#define BATTLESHIP_COMMON_H

#include <stdbool.h>
#include "../utils/types.h"
#include "../game/board.h"
#include "../game/ship.h"

void placeRandomShips(Board* board, const int shipLengths[], int shipLengthsLength);

#endif /* BATTLESHIP_COMMON_H */
