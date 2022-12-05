#ifndef BATTLESHIP_SHIP_H
#define BATTLESHIP_SHIP_H

#include "types.h"

void initShip(Ship* ship, Ship_Type type, Orientation orientation);
void initShipWithName(Ship* ship, Ship_Type type, Orientation orientation, char* name);
void setShipState(Ship* ship, int index);
void freeShip(Ship* ship);

#endif
