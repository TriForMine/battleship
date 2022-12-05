#include "tile.h"
#include "ship.h"
#include <stdio.h>
#include <stdlib.h>

void initTile(Tile* tile) {
    tile->state = WATER;
    tile->ship = NULL;
}

void setTileState(Tile* tile, enum State state) {
    tile->state = state;
}

void setTileShip(Tile* tile, Ship* ship) {
    tile->state = SHIP;
    tile->ship = ship;
}

void resetTile(Tile* tile) {
    tile->state = WATER;
    tile->ship = NULL;
}

void freeTile(Tile* tile) {
    if (tile->ship != NULL) {
        freeShip(tile->ship);
        free(tile->ship);
    }
    tile->ship = NULL;
}
