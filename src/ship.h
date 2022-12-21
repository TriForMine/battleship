#ifndef BATTLESHIP_SHIP_H
#define BATTLESHIP_SHIP_H

#include "types.h"
#include <stdbool.h>
#include <string.h>

Ship* createShip(Ship_Type type, Orientation orientation);
Ship* createShipWithName(Ship_Type type, Orientation orientation, char* name);
void setShipState(Ship* ship, int index);
void freeShip(Board* board, Ship* ship);
Ship* getShipWithCoordinate(Board* board, Coordinate coordinate);
Ship* getShipWithName(Board* board, char* name);
bool shipExists(Board* board, char* name);

#endif
