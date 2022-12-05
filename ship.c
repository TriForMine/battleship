#include "ship.h"
#include <stdio.h>
#include <stdlib.h>

void initShip(Ship* ship, enum Ship_Type type, enum Orientation orientation) {
    int i;

    ship->type = type;
    ship->orientation = orientation;
    ship->name = NULL;
    ship->hits = 0;
}

void initShipWithName(Ship* ship, enum Ship_Type type, enum Orientation orientation, char* name) {
    initShip(ship, type, orientation);
    ship->name = name;
}

void freeShip(Ship* ship) {
    if (ship->name != NULL)
        free(ship->name);
}