#ifndef BATTLESHIP_SHIP_H
#define BATTLESHIP_SHIP_H

#include "../utils/types.h"
#include "../utils/hmap.h"
#include <stdbool.h>
#include <string.h>

Ship* createShip(Ship_Type type, Orientation orientation, char owner);
Ship* createShipWithName(Board* board, Ship_Type type, Orientation orientation, char owner, char* name);
bool isShipSunk(Ship* ship);
bool isShipHitAtCoordinate(Ship* ship, Coordinate coordinate);
void freeShip(Board* board, Ship* ship);

#endif
