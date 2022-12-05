#ifndef BATTLESHIP_BOARD_H
#define BATTLESHIP_BOARD_H

#include "types.h"
#include "tile.h"

/* Board */
void initBoard(Board* board, int WIDTH, int HEIGHT);
void resetBoard(Board* board);
void freeBoard(Board* board);

/* Ship Placements */
void placeShip(Board* board, Ship* ship, Coordinate position);
void placeMine(Board* board, Coordinate position);

/* Ship Getters */
Ship* getShip(Board* board, Coordinate position);

/* Printing */
void printBoard(Board* board);

/* Movements */
void moveUp(Board* board, Coordinate position);
void moveDown(Board* board, Coordinate position);
void moveLeft(Board* board, Coordinate position);
void moveRight(Board* board, Coordinate position);

/* Fire */
void fire(Board* board, Coordinate position);

#endif
