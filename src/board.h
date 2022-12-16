#ifndef BATTLESHIP_BOARD_H
#define BATTLESHIP_BOARD_H

#include "helpers.h"
#include "tile.h"
#include "types.h"

/* Board */
Board* createBoard(int WIDTH, int HEIGHT);
void resetBoard(Board* board);
void freeBoard(Board* board);
Tile getTile(Board* board, Coordinate coordinate);
State getTileState(Board* board, Coordinate coordinate);

/* Ship Placements */
void placeShip(Board* board, Ship* ship, Coordinate position);
void placeMine(Board* board, Coordinate position);

/* Ship Getters */
Ship* getShip(Board* board, Coordinate position);

/* Printing */
void printBoard(Board* board);

/* Movements */
void moveShip(Board* board, Ship* ship, Direction direction);
void moveUp(Board* board, Coordinate position);
void moveDown(Board* board, Coordinate position);
void moveLeft(Board* board, Coordinate position);
void moveRight(Board* board, Coordinate position);

/* Fire */
void fireAt(Board* board, Coordinate position);

#endif
