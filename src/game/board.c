#include "board.h"
#include "ship.h"

/* Board */
Board* createBoard(unsigned int width, unsigned int height) {
    Board* board = malloc_prof(sizeof(Board));
    unsigned int x, y;

    board->WIDTH = width;
    board->HEIGHT = height;
    board->tiles = malloc_prof(sizeof(Tile) * board->WIDTH * board->HEIGHT);
    board->ships_by_name = createDictionary((unsigned long)board->WIDTH * (unsigned long)board->HEIGHT);
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
        freeDictionary(board->ships_by_name);
        board->ships_by_name = NULL;
    }
    free_prof(board);
}

void updateBoardShip(Board* board, Ship* ship, Coordinate oldPos) {
    unsigned int i;

    /* Remove the ship from the old position */
    for (i = 0; i < ship->type; ++i) {
        Coordinate current;
        if (ship->orientation == HORIZONTAL) {
            current.x = oldPos.x + i;
            current.y = oldPos.y;
        } else {
            current.x = oldPos.x;
            current.y = oldPos.y + i;
        }
        removeTileShip(getTile(board, current));
    }

    /* Set the ship at the new position */
    for (i = 0; i < ship->type; ++i) {
        Coordinate current;
        if (ship->orientation == HORIZONTAL) {
            current.x = ship->head.x + i;
            current.y = ship->head.y;
        } else {
            current.x = ship->head.x;
            current.y = ship->head.y + i;
        }
        setTileShip(getTile(board, current), ship);
    }
}

Tile* getTile(Board* board, Coordinate coordinate) { return &board->tiles[coordinate.x + coordinate.y * board->WIDTH]; }

State getTileState(Board* board, Coordinate coordinate) { return getTile(board, coordinate)->state; }

/* Checks */
bool isCoordinateValid(Board* board, Coordinate coordinate) {
    return coordinate.x < board->WIDTH && coordinate.y < board->HEIGHT;
}

/* Ship Placements */
void placeShip(Board* board, Ship* ship, Coordinate position) {
    unsigned int i;
    Coordinate current;

    if (isCoordinateValid(board, position) == false) {
        RAISE_ERROR(ERR_INVALID_SHIP_POSITION);
    }

    if (ship->orientation == HORIZONTAL) {
        for (i = 0; i < ship->type; ++i) {
            current.x = position.x + i;
            current.y = position.y;

            if (isCoordinateValid(board, current) == false) {
                RAISE_ERROR(ERR_INVALID_SHIP_POSITION);
            }

            if (getTileState(board, current) == SHIP) {
                RAISE_ERROR(ERR_SHIP_OVERLAP);
            }

            setTileShip(getTile(board, createCoordinate(current.x, current.y)), ship);
        }
    } else {
        for (i = 0; i < ship->type; ++i) {
            current.x = position.x;
            current.y = position.y + i;

            if (isCoordinateValid(board, current) == false) {
                RAISE_ERROR(ERR_INVALID_SHIP_POSITION);
            }

            if (getTileState(board, current) == SHIP) {
                RAISE_ERROR(ERR_SHIP_OVERLAP);
            }

            setTileShip(getTile(board, createCoordinate(current.x, current.y)), ship);
        }
    }

    ship->head = position;
}

void placeMine(Board* board, Coordinate position) { setTileState(getTile(board, position), MINE); }

/* Ship Getters */
Ship* getShip(Board* board, Coordinate position) { return getTile(board, position)->ship; }

Ship* getShipWithName(Board* board, char* name) { return dictionaryGet(board->ships_by_name, name); }

bool shipExists(Board* board, char* name) { return getShipWithName(board, name) != NULL; }

/* Printing */
void printBoard(Board* board, bool showShips) {
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
                        if (ship->hits == (1 << ship->type) - 1) {
                            printf(ANSI_COLOR_CYAN "□" ANSI_COLOR_RESET " ");
                        } else if (ship->hits >> (x - head.x) & 1) {
                            printf(ANSI_COLOR_RED "□" ANSI_COLOR_RESET " ");
                        } else if (showShips) {
                            printf("■ ");
                        } else {
                            printf(ANSI_COLOR_BLUE "~ " ANSI_COLOR_RESET);
                        }
                    } else {
                        if (ship->hits == (1 << ship->type) - 1) {
                            printf(ANSI_COLOR_CYAN "□" ANSI_COLOR_RESET " ");
                        } else if (ship->hits >> (y - head.y) & 1) {
                            printf(ANSI_COLOR_RED "□" ANSI_COLOR_RESET " ");
                        } else if (showShips) {
                            printf("■ ");
                        } else {
                            printf(ANSI_COLOR_BLUE "~ " ANSI_COLOR_RESET);
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
    /* Save the original position of the ship*/
    Coordinate originalPos = ship->head;
    Coordinate newPos = ship->head;

    if (isShipSunk(ship)) {
        RAISE_ERROR(ERR_SUNK_SHIP);
    }

    /* Update the position of the ship's head based on the direction*/
    switch (direction) {
        case UP:
            newPos.y -= 1;
            break;
        case DOWN:
            newPos.y += 1;
            break;
        case LEFT:
            newPos.x -= 1;
            break;
        case RIGHT:
            newPos.x += 1;
            break;
    }

    /* Check for collision on the whole ship*/
    if (handleCollision(board, ship, newPos)) {
        return;
    }

    /* Update the ship's position on the board*/
    ship->head = newPos;

    /* Update the board to reflect the ship's new position*/
    updateBoardShip(board, ship, originalPos);
}

/* check if moving the whole ship to the new position will collide with an existing bot, if it does couts hits for both bot and don't move them
 * return true if the ship collides with another ship
 */
bool handleCollision(Board* board, Ship* ship, Coordinate position) {
    unsigned int i;
    Coordinate current;
    bool collision = false;

    for (i = 0; i < ship->type; ++i) {
        current.x = position.x + (ship->orientation == HORIZONTAL ? i : 0);
        current.y = position.y + (ship->orientation == VERTICAL ? i : 0);

        if (getTileState(board, current) == SHIP) {
            Ship* otherShip = getShip(board, current);
            if (otherShip != ship) {
                collision = true;
                ship->hits |= 1 << i;
                otherShip->hits |= 1 << ((ship->orientation == HORIZONTAL) ? (current.x - otherShip->head.x)
                                                                           : (current.y - otherShip->head.y));
            }
        } else if (getTileState(board, current) == MINE) {
            ship->hits |= 1 << i;
        }
    }

    return collision;
}

void moveUp(Board* board, Coordinate position) {
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
    Ship* ship;

    if (isCoordinateValid(board, position) == false) {
        RAISE_ERROR(ERR_INVALID_COORDINATES);
    }

    if (getTileState(board, position) == MINE) {
        RAISE_ERROR(ERR_COORDINATE_ALREADY_HIT);
    }

    ship = getShip(board, position);

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
