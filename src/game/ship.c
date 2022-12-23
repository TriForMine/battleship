#include <stdio.h>
#include "ship.h"
#include "../utils/stdprof.h"
#include "../utils/helpers.h"
#include "board.h"

Ship* createShip(Ship_Type type, Orientation orientation) {
    Ship* ship = malloc_prof(sizeof(Ship));

    ship->type = type;
    ship->orientation = orientation;
    ship->name = NULL;
    ship->hits = 0;

    return ship;
}

Ship* createShipWithName(Board* board, Ship_Type type, Orientation orientation, char* name) {
    Ship* ship = createShip(type, orientation);
    ship->name = name;
    dictionarySet(board->ships_by_name, name, ship);
    return ship;
}

bool isShipSunk(Ship* ship) { return ship->hits == (1 << (int)ship->type) - 1; }

bool isShipHitAtCoordinate(Ship* ship, Coordinate coordinate) {
    Coordinate head = ship->head;

    return (ship->hits & (1 << (ship->orientation == HORIZONTAL ? coordinate.x - head.x : coordinate.y - head.y))) != 0;
}

unsigned int getShipLength(Ship_Type type) {
    switch (type) {
        case CARRIER:
            return 5;
        case BATTLESHIP:
            return 4;
        case CRUISER:
            return 3;
        case DESTROYER:
            return 2;
        case FRIGATE:
            return 1;
        default:
            return 0;
    }
}

void freeShip(Board* board, Ship* ship) {
    unsigned int i;

    if (ship != NULL) {
        if (ship->name != NULL) {
            free_prof(ship->name);
            ship->name = NULL;
        }

        /* Set all other cases to NULL to free the ship multiple times */
        for (i = 0; i < ship->type; i++) {
            setTileShip(getTile(board, createCoordinate(ship->head.x + i * (ship->orientation == HORIZONTAL),
                                                        (ship->head.y + i * (ship->orientation == VERTICAL)))),
                        NULL);
        }

        free_prof(ship);
    }
}