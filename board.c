#include "board.h"
#include "ship.h"
#include "tile.h"
#include <stdio.h>
#include <stdlib.h>

/* Board */
void initBoard(Board* board, int WIDTH, int height) {
    int x, y;

    board->WIDTH = WIDTH;
    board->HEIGHT = height;
    board->tiles = malloc(sizeof(Tile) * board->WIDTH * board->HEIGHT);
    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            initTile(&board->tiles[x + y * board->WIDTH]);
        }
    }
}

void resetBoard(Board* board) {
    int x, y;

    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            resetTile(&board->tiles[x + y * board->WIDTH]);
        }
    }
}

void freeBoard(Board* board) {
    int x, y;

    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            freeTile(&board->tiles[x + y * board->WIDTH]);
        }
    }
    free(board->tiles);
}

/* Ship Placements */
void placeShip(Board* board, Ship* ship, Coordinate position) {
    int i;
    Coordinate current;

    if (ship->orientation == HORIZONTAL) {
        for (i = 0; i < ship->type; ++i) {
            current.x = position.x + i;
            current.y = position.y;
            setTileShip(&board->tiles[current.x + current.y * board->WIDTH], ship);
        }
    } else {
        for (i = 0; i < ship->type; ++i) {
            current.x = position.x;
            current.y = position.y + i;
            setTileShip(&board->tiles[current.x + current.y * board->WIDTH], ship);
        }
    }
}

void placeMine(Board* board, Coordinate position) {
    setTileState(&board->tiles[position.x + position.y * board->WIDTH], MINE);
}

/* Ship Getters */
Ship* getShip(Board* board, Coordinate position) {
    return board->tiles[position.x + position.y * board->WIDTH].ship;
}

/* Printing */
void printBoard(Board* board) {

    Ship* ship;
    Coordinate head;
    int x, y;

    printf("  ");
    for (x = 0; x < board->WIDTH; ++x) {
        printf("%d ", x);
    }
    printf("\n");
    for (y = 0; y < board->HEIGHT; ++y) {
        printf("%c ", 65 + y);
        for (x = 0; x < board->WIDTH; ++x) {
            switch (board->tiles[x + y * board->WIDTH].state) {
                case WATER:
                    printf("~ ");
                    break;
                case SHIP:
                    ship = getShip(board, (Coordinate){x, y});
                    head=ship->head;
                    /* affiche le hits du bateau*/

                    if (ship->orientation == HORIZONTAL) {

                        if (ship->hits>>x-head.x&1) {
                            printf("\033[31m☒\033[0m ");
                        } else {
                            printf("\033[31m■\033[0m ");
                        }
                    }
                    else {
                        if (ship->hits>>y-head.y&1) {
                            printf("\033[31m☒\033[0m ");
                        } else {
                            printf("\033[31m■\033[0m ");
                        }
                    }

                    break;
                case MINE:
                    printf("⚠ ");
                    break;
                default:
                    printf("  ");
                    break;
            }
        }
        printf("\n");
    }
}

/* Movements */
void moveUp(Board* board, Coordinate position) {
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.y > 0) {
            resetTile(&board->tiles[position.x + position.y * board->WIDTH]);
            setTileShip(&board->tiles[position.x + (position.y - 1) * board->WIDTH], ship);
        }
    }
}

void moveDown(Board* board, Coordinate position) {
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.y < board->HEIGHT - 1) {
            resetTile(&board->tiles[position.x + position.y * board->WIDTH]);
            setTileShip(&board->tiles[position.x + (position.y + 1) * board->WIDTH], ship);
        }
    }
}

void moveLeft(Board* board, Coordinate position) {
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.x > 0) {
            resetTile(&board->tiles[position.x + position.y * board->WIDTH]);
            setTileShip(&board->tiles[(position.x - 1) + position.y * board->WIDTH], ship);
        }
    }
}

void moveRight(Board* board, Coordinate position) {
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.x < board->WIDTH - 1) {
            resetTile(&board->tiles[position.x + position.y * board->WIDTH]);
            setTileShip(&board->tiles[(position.x + 1) + position.y * board->WIDTH], ship);
        }
    }
}

/* Fire */
void fire(Board* board, Coordinate position) {
    Coordinate head;
    Ship* ship = getShip(board, position);

    if (ship != NULL) {
        head = ship->head;
        /* If the ship is hit, update the hit bitfield */
        if (ship->orientation == HORIZONTAL) {
            ship->hits |= 1 << (position.x - head.x);
        } else {
            ship->hits |= 1 << (position.y - head.y);
        }
    } else {
        placeMine(board, position);
    }
}

