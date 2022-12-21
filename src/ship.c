#include "ship.h"
#include "stdprof.h"

Ship*
createShip(Ship_Type type, Orientation orientation) {
    Ship* ship = malloc_prof(sizeof(Ship));

    ship->type = type;
    ship->orientation = orientation;
    ship->name = NULL;
    ship->hits = 0;

    return ship;
}

Ship*
createShipWithName(Ship_Type type, Orientation orientation, char* name) {
    Ship* ship = createShip(type, orientation);
    ship->name = name;
    return ship;
}

void
freeShip(Board* board, Ship* ship) {
    int i;

    if (ship != NULL) {
        if (ship->name != NULL) {
            free_prof(ship->name);
            ship->name = NULL;
        }

        /* Set all other cases to NULL to free the ship multiple times */
        for (i = 0; i < ship->type; i++) {
            board
                ->tiles[ship->head.x + i * (ship->orientation == HORIZONTAL)
                        + (ship->head.y + i * (ship->orientation == VERTICAL)) * board->HEIGHT]
                .ship = NULL;
        }

        free_prof(ship);
    }
}

Ship*
getShipWithCoordinate(Board* board, Coordinate coordinate) {
    return board->tiles[coordinate.x + coordinate.y * board->WIDTH].ship;
}

Ship*
getShipWithName(Board* board, char* name) {
    int i, j;
    Ship* ship = NULL;
    for (i = 0; i < board->WIDTH; i++) {
        for (j = 0; j < board->HEIGHT; j++) {
            if (board->tiles[i + j * board->HEIGHT].ship != NULL) {
                if (strcmp(board->tiles[i + j * board->HEIGHT].ship->name, name) == 0) {
                    ship = board->tiles[i + j * board->HEIGHT].ship;
                    return ship;
                }
            }
        }
    }
    return ship;
}

bool
shipExists(Board* board, char* name) {
    return getShipWithName(board, name) != NULL;
}