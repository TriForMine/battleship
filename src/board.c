#include "board.h"
#include "const.h"
#include "tile.h"
#include "stdprof.h"
#include <stdio.h>

/* Board */
Board* createBoard(int width, int height) {
    Board* board = malloc_prof(sizeof(Board));
    unsigned int x, y;

    board->WIDTH = width;
    board->HEIGHT = height;
    board->tiles = malloc_prof(sizeof(Tile) * board->WIDTH * board->HEIGHT);
    board->ships_by_name = dictionary_create(board->WIDTH * board->HEIGHT);
    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            initTile(getTile(board, createCoordinate(x, y)));
        }
    }

    return board;
}

void resetBoard(Board* board) {
    unsigned int x, y;

    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            resetTile(getTile(board, createCoordinate(x, y)));
        }
    }
}

void freeBoard(Board* board) {
    unsigned int x, y;

    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            freeTile(board, getTile(board, createCoordinate(x, y)));
        }
    }
    if (board->tiles != NULL) {
        free_prof(board->tiles);
    }

    if (board->ships_by_name != NULL) {
        dictionary_free(board->ships_by_name);
        board->ships_by_name = NULL;
    }
    free_prof(board);
}

Tile* getTile(Board* board, Coordinate coordinate) { return &board->tiles[coordinate.x + coordinate.y * board->WIDTH]; }

State getTileState(Board* board, Coordinate coordinate) { return getTile(board, coordinate)->state; }

/* Ship Placements */
void placeShip(Board* board, Ship* ship, Coordinate position) {
    unsigned int i;
    Coordinate current;

    if (ship->orientation == HORIZONTAL) {
        for (i = 0; i < ship->type; ++i) {
            current.x = position.x + i;
            current.y = position.y;
            setTileShip(getTile(board, createCoordinate(current.x, current.y)), ship);
        }
    } else {
        for (i = 0; i < ship->type; ++i) {
            current.x = position.x;
            current.y = position.y + i;
            setTileShip(getTile(board, createCoordinate(current.x, current.y)), ship);
        }
    }

    ship->head = position;
}

void placeMine(Board* board, Coordinate position) { setTileState(getTile(board, position), MINE); }

/* Ship Getters */
Ship* getShip(Board* board, Coordinate position) { return getTile(board, position)->ship; }

Ship* getShipWithName(Board* board, char* name) { return dictionary_lookup(board->ships_by_name, name); }

bool shipExists(Board* board, char* name) { return getShipWithName(board, name) != NULL; }

/* Printing */
void printBoard(Board* board) {

    Ship* ship;
    Coordinate head;
    unsigned int x, y;

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
                    ship = getShip(board, createCoordinate(x, y));
                    head = ship->head;

                    if (ship->orientation == HORIZONTAL) {

                        if (ship->hits >> (x - head.x) & 1) {
                            printf(ANSI_COLOR_RED "□" ANSI_COLOR_RESET " ");
                        } else {
                            printf("■ ");
                        }
                    } else {
                        if (ship->hits >> (y - head.y) & 1) {
                            printf(ANSI_COLOR_RED "□" ANSI_COLOR_RESET " ");
                        } else {
                            printf("■ ");
                        }
                    }

                    break;
                case MINE:
                    printf(ANSI_COLOR_YELLOW "⚠" ANSI_COLOR_RESET " ");
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
void moveShip(Board* board, Ship* ship, Direction direction) {
    unsigned int i;
    Coordinate current;
    Coordinate position;
    position = ship->head;

    switch (direction) {
        case UP:
            position.y -= 1;
            break;
        case DOWN:
            position.y += 1;
            break;
        case LEFT:
            position.x -= 1;
            break;
        case RIGHT:
            position.x += 1;
            break;
    }

    if (ship->orientation == HORIZONTAL) {
        switch (direction) {
            case UP:
            case DOWN:
                if (handleCollision(board, ship, position)) {
                    return;
                }

                for (i = 0; i < ship->type; ++i) {
                    current.x = position.x + i;
                    current.y = position.y;

                    if (getTileState(board, current) == MINE) {
                        ship->hits |= 1 << i;
                    }

                    removeTileShip(getTile(board, createCoordinate(current.x, ship->head.y)));
                    setTileShip(getTile(board, createCoordinate(current.x, current.y)), ship);
                }
                break;
            case LEFT:
                if (handleCollision(board, ship, position)) {
                    return;
                }

                if (getTileState(board, position) == MINE) {
                    ship->hits |= 1 << 0;
                }

                removeTileShip(getTile(board, createCoordinate((ship->head.x + ship->type - 1), ship->head.y)));
                setTileShip(getTile(board, position), ship);
                break;
            case RIGHT:
                if (handleCollision(board, ship, position)) {
                    return;
                }

                if (getTileState(board, createCoordinate((ship->head.x + ship->type), ship->head.y)) == MINE) {
                    ship->hits |= 1 << (ship->type - 1);
                }

                removeTileShip(getTile(board, createCoordinate(ship->head.x, ship->head.y)));
                setTileShip(getTile(board, createCoordinate((ship->head.x + ship->type), +position.y)), ship);
                break;
        }
    } else {
        switch (direction) {
            case UP:
                if (handleCollision(board, ship, position)) {
                    return;
                }

                if (getTileState(board, position) == MINE) {
                    ship->hits |= 1 << 0;
                }

                removeTileShip(getTile(board, createCoordinate(ship->head.x, (ship->head.y + ship->type - 1))));
                setTileShip(getTile(board, position), ship);
                break;
            case DOWN:
                if (handleCollision(board, ship, position)) {
                    return;
                }

                if (getTileState(board, createCoordinate(ship->head.x, ship->head.y + ship->type)) == MINE) {
                    ship->hits |= 1 << (ship->type - 1);
                }

                removeTileShip(getTile(board, createCoordinate(ship->head.x, ship->head.y)));
                setTileShip(getTile(board, createCoordinate(position.x, (position.y + ship->type - 1))), ship);
                break;
            case LEFT:
            case RIGHT:
                if (handleCollision(board, ship, position)) {
                    return;
                }

                for (i = 0; i < ship->type; ++i) {
                    current.x = position.x;
                    current.y = position.y + i;

                    if (getTileState(board, current) == MINE) {
                        ship->hits |= 1 << i;
                    }

                    removeTileShip(getTile(board, createCoordinate(ship->head.x, (ship->head.y + i))));
                    setTileShip(getTile(board, createCoordinate(current.x, current.y)), ship);
                }
                break;
        }
    }

    ship->head = position;
}

/* check if moving the whole ship to the new position will collide with an existing bot, if it does couts hits for both bot and don't move them
 * return true if the ship collides with another ship
 */
bool handleCollision(Board* board, Ship* ship, Coordinate position) {
    unsigned int i;
    Coordinate current;
    bool collision = false;

    if (ship->orientation == HORIZONTAL) {
        for (i = 0; i < ship->type; ++i) {
            current.x = position.x + i;
            current.y = position.y;

            if (getTileState(board, current) == SHIP) {
                collision = true;
                ship->hits |= 1 << i;
                getShip(board, current)->hits |= 1 << (current.x - getShip(board, current)->head.x);
            }
        }
    } else {
        for (i = 0; i < ship->type; ++i) {
            current.x = position.x;
            current.y = position.y + i;

            if (getTileState(board, current) == SHIP) {
                collision = true;
                ship->hits |= 1 << i;
                getShip(board, current)->hits |= 1 << (current.y - getShip(board, current)->head.y);
            }
        }
    }

    return collision;
}

void moveUp(Board* board, Coordinate position) {
    int i;
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.y > 0) {
            moveShip(board, ship, UP);
        }
    }
}

void moveDown(Board* board, Coordinate position) {
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.y < board->HEIGHT - 1) {
            moveShip(board, ship, DOWN);
        }
    }
}

void moveLeft(Board* board, Coordinate position) {
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.x > 0) {
            moveShip(board, ship, LEFT);
        }
    }
}

void moveRight(Board* board, Coordinate position) {
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.x < board->WIDTH - 1) {
            moveShip(board, ship, RIGHT);
        }
    }
}

/* Fire */
void fireAt(Board* board, Coordinate position) {
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
