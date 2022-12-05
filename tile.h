#ifndef BATTLESHIP_TILE_H
#define BATTLESHIP_TILE_H

#include "types.h"

void initTile(Tile* tile);
void setTileState(Tile* tile, enum State state);
void setTileShip(Tile* tile, Ship* ship);
void resetTile(Tile* tile);
void freeTile(Tile* tile);

#endif
