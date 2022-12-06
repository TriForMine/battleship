#include "tile.h"
#include "ship.h"
#include <stdio.h>
#include <stdlib.h>

void initTile(Tile* tile) {
    tile->state = WATER;
    tile->ship = NULL;
}

void setTileState(Tile* tile, State state) {
    tile->state = state;
}

void setTileShip(Tile* tile, Ship* ship) {
    tile->state = SHIP;
    tile->ship = ship;
}

void removeTileShip(Tile* tile) {
    tile->state = WATER;
    tile->ship = NULL;
}


void resetTile(Tile* tile) {
    if(tile->state == SHIP) {
        tile->ship = NULL;
    }
    tile->state = WATER;
}

void freeTile(Tile* tile) {
    if (tile->ship != NULL) {
        freeShip(tile->ship);
    }

    tile->ship = NULL;
}
