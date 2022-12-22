#include <stdio.h>
#include "ship.h"
#include "../utils/stdprof.h"
#include "../utils/helpers.h"
#include "board.h"

Ship* createShip(Ship_Type type, Orientation orientation, char owner) {
    Ship* ship = malloc_prof(sizeof(Ship));

    ship->type = type;
    ship->orientation = orientation;
    ship->name = NULL;
    ship->hits = 0;
    ship->owner = owner;

    return ship;
}

Ship* createShipWithName(Board* board, Ship_Type type, Orientation orientation, char owner, char* name) {
    Ship* ship = createShip(type, orientation, owner);
    ship->name = name;
    dictionarySet(board->ships_by_name, name, ship);
    return ship;
}

bool isShipSunk(Ship* ship) { return ship->hits == (1 << (int)ship->type) - 1; }

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
