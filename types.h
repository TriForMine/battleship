/*
 * This file contains all the types used in the program.
 * It is included in every file that needs to use these types.
 */

#ifndef BATTLESHIP_TYPES_H
#define BATTLESHIP_TYPES_H

enum State {
    WATER = 0,
    MINE = 1,
    SHIP = 2
};

enum Ship_Type {
    FRIGATE = 1,
    DESTROYER = 2,
    CRUISER = 3,
    BATTLESHIP = 4,
    CARRIER = 5
};

enum Orientation {
    HORIZONTAL,
    VERTICAL
};

typedef struct {
    int x;
    int y;
} Coordinate;

/* Ship */
typedef struct {
    enum Ship_Type type;
    enum Orientation orientation;
    char* name;
    /* Bitfields of hits */
    int hits;
    Coordinate head;
} Ship;

/* Tile */
typedef struct {
    Ship* ship;
    enum State state;
} Tile;

/* Board */
typedef struct {
    Tile* tiles;
    int WIDTH;
    int HEIGHT;
} Board;

#endif
