#ifndef BATTLESHIP_SHIP_H
#define BATTLESHIP_SHIP_H

#include "../utils/types.h"
#include "../utils/hmap.h"
#include <stdbool.h>
#include <string.h>

Ship* createShip(Ship_Type type, Orientation orientation);
Ship* createShipWithName(Board* board, Ship_Type type, Orientation orientation, char* name);
bool isShipSunk(Ship* ship);
bool isShipHitAtCoordinate(Ship* ship, Coordinate coordinate);
unsigned int getShipLength(Ship_Type type);
void freeShip(Board* board, Ship* ship);

#endif
