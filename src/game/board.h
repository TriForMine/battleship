#ifndef BATTLESHIP_BOARD_H
#define BATTLESHIP_BOARD_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "../utils/const.h"
#include "../utils/colors.h"
#include "../utils/error.h"
#include "tile.h"
#include "../utils/stdprof.h"
#include "../utils/helpers.h"
#include "../utils/hmap.h"
#include "../utils/types.h"

/* Board */
Board* createBoard(unsigned int WIDTH, unsigned int HEIGHT);
void resetBoard(Board* board);
void freeBoard(Board* board);
void updateBoardShip(Board* board, Ship* ship, Coordinate newPos);
void moveShip(Board* board, Ship* ship, Direction direction);
Tile* getTile(Board* board, Coordinate coordinate);
State getTileState(Board* board, Coordinate coordinate);
void getNearbyCoordinates(Board* board, Coordinate current, Coordinate* list, unsigned int* size);

/* Checks */
bool isCoordinateValid(Board* board, Coordinate coordinate);
bool isTileMine(Board* board, Coordinate coordinate);
bool isTileShip(Board* board, Coordinate coordinate);
bool isTileUnknown(Board* board, Coordinate coordinate);
bool isTileHit(Board* board, Coordinate coordinate);
bool isTileFired(Board* board, Coordinate coordinate);

/* Ship Placements */
void placeShip(Board* board, Ship* ship, Coordinate position);
void placeMine(Board* board, Coordinate position);

/* Ship Getters */
Ship* getShip(Board* board, Coordinate position);
Ship* getShipWithName(Board* board, char* name);

/* Printing */
void printBoard(Board* board, bool showShips);

/* Movements */
bool handleCollision(Board* board, Ship* ship, Coordinate position);

/* Fire */
void fireAt(Board* board, Coordinate position);

#endif
