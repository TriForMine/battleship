#ifndef BATTLESHIP_BOARD_H
#define BATTLESHIP_BOARD_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "const.h"
#include "error.h"
#include "tile.h"
#include "stdprof.h"
#include "helpers.h"
#include "hmap.h"
#include "types.h"

/* Board */
Board* createBoard(unsigned int WIDTH, unsigned int HEIGHT);
void resetBoard(Board* board);
void freeBoard(Board* board);
void updateBoardShip(Board* board, Ship* ship, Coordinate newPos);
void moveShip(Board* board, Ship* ship, Direction direction);
Tile* getTile(Board* board, Coordinate coordinate);
State getTileState(Board* board, Coordinate coordinate);

/* Checks */
bool isCoordinateValid(Board* board, Coordinate coordinate);

/* Ship Placements */
void placeShip(Board* board, Ship* ship, Coordinate position);
void placeMine(Board* board, Coordinate position);

/* Ship Getters */
Ship* getShip(Board* board, Coordinate position);
Ship* getShipWithName(Board* board, char* name);
bool shipExists(Board* board, char* name);

/* Printing */
void printBoard(Board* board);

/* Movements */
bool handleCollision(Board* board, Ship* ship, Coordinate position);
void moveUp(Board* board, Coordinate position);
void moveDown(Board* board, Coordinate position);
void moveLeft(Board* board, Coordinate position);
void moveRight(Board* board, Coordinate position);

/* Fire */
void fireAt(Board* board, Coordinate position);

#endif
