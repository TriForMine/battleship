#ifndef BATTLESHIP_SHIP_H
#define BATTLESHIP_SHIP_H

#include "types.h"

Ship* createShip(Ship_Type type, Orientation orientation);
Ship* createShipWithName(Ship_Type type, Orientation orientation, char* name);
void setShipState(Ship* ship, int index);
void freeShip(Ship* ship);

#endif
