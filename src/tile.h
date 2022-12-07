#ifndef BATTLESHIP_TILE_H
#define BATTLESHIP_TILE_H

#include "types.h"

void initTile(Tile* tile);
void setTileState(Tile* tile, State state);
void setTileShip(Tile* tile, Ship* ship);
void removeTileShip(Tile* tile);
void resetTile(Tile* tile);
void freeTile(Board* board, Tile* tile);

#endif
