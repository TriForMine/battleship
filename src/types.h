/*
 * This file contains all the types used in the program.
 * It is included in every file that needs to use these types.
 */

#ifndef BATTLESHIP_TYPES_H
#define BATTLESHIP_TYPES_H

#include <stdlib.h>
#include <stdbool.h>
#include "hmap.h"

typedef enum { WATER = 0, MINE = 1, SHIP = 2 } State;

typedef enum { FRIGATE = 1, DESTROYER = 2, CRUISER = 3, BATTLESHIP = 4, CARRIER = 5 } Ship_Type;

typedef enum { HORIZONTAL, VERTICAL } Orientation;

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

typedef enum { Player1, Player2 } GameTurn;

typedef struct {
    unsigned int x;
    unsigned int y;
} Coordinate;

/* Ship */
typedef struct {
    Ship_Type type;
    Orientation orientation;
    char* name;
    /* Bitfields of hits */
    int hits;
    Coordinate head;
    char owner;
} Ship;

/* Tile */
typedef struct {
    Ship* ship;
    State state;
} Tile;

/* Board */
typedef struct {
    Tile* tiles;
    unsigned int WIDTH;
    unsigned int HEIGHT;
    Dictionary* ships_by_name;
} Board;

/* Game */
typedef struct {
    Board* board1;
    Board* board2;
    GameTurn turn;
    bool single_board;
    unsigned int remaining_ships[2];
} Game;

#endif
