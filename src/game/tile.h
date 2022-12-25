#ifndef BATTLESHIP_TILE_H
#define BATTLESHIP_TILE_H

#include "../utils/types.h"

void initTile(Tile* tile);
void setTileState(Tile* tile, State state);
void setTileShip(Tile* tile, Ship* ship);
void removeTileShip(Tile* tile);
void resetTile(Board* board, Tile* tile);
void freeTile(Board* board, Tile* tile);

#endif
