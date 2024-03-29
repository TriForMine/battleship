#include "board.h"
#include "ship.h"

/* Board */
Board *createBoard(unsigned int width, unsigned int height) {
    Board *board = malloc_prof(sizeof(Board));
    unsigned int x, y;

    board->WIDTH = width;
    board->HEIGHT = height;
    board->tiles = malloc_prof(sizeof(Tile) * board->WIDTH * board->HEIGHT);
    board->ships_by_name = createDictionary((unsigned long) board->WIDTH * (unsigned long) board->HEIGHT);
    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            initTile(getTile(board, createCoordinate(x, y)));
        }
    }

    return board;
}

void resetBoard(Board *board) {
    unsigned int x, y;

    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            resetTile(board, getTile(board, createCoordinate(x, y)));
        }
    }

    clearDictionary(board->ships_by_name);
}

void freeBoard(Board *board) {
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

void updateBoardShip(Board *board, Ship *ship, Coordinate oldPos) {
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

Tile *getTile(Board *board, Coordinate coordinate) { return &board->tiles[coordinate.x + coordinate.y * board->WIDTH]; }

State getTileState(Board *board, Coordinate coordinate) { return getTile(board, coordinate)->state; }

void getNearbyCoordinates(Board *board, Coordinate current, Coordinate *list, unsigned int *size) {
    unsigned int i = 0;

    if (current.x > 0) {
        list[i++] = createCoordinate(current.x - 1, current.y);
    }
    if (current.x < board->WIDTH - 1) {
        list[i++] = createCoordinate(current.x + 1, current.y);
    }
    if (current.y > 0) {
        list[i++] = createCoordinate(current.x, current.y - 1);
    }
    if (current.y < board->HEIGHT - 1) {
        list[i++] = createCoordinate(current.x, current.y + 1);
    }

    *size = i;
}

/* Checks */
bool isCoordinateValid(Board *board, Coordinate coordinate) {
    return coordinate.x < board->WIDTH && coordinate.y < board->HEIGHT;
}

bool isTileMine(Board *board, Coordinate coordinate) {
    State state = getTileState(board, coordinate);
    return state == MINE;
}

bool isTileShip(Board *board, Coordinate coordinate) {
    State state = getTileState(board, coordinate);
    return state == SHIP;
}

bool isTileUnknown(Board *board, Coordinate coordinate) {
    State state = getTileState(board, coordinate);
    return state == WATER || (state == SHIP && isShipHitAtCoordinate(getShip(board, coordinate), coordinate) == false);
}

bool isTileHit(Board *board, Coordinate coordinate) {
    Ship *ship = getShip(board, coordinate);
    return ship != NULL && isShipHitAtCoordinate(ship, coordinate) && isShipSunk(ship) == false;
}

bool isTileFired(Board *board, Coordinate coordinate) {
    return isTileMine(board, coordinate) || isTileHit(board, coordinate);
}

bool isShipAlive(Board *board, Ship_Type ship_type) {
    unsigned int x, y;

    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            Coordinate coordinate = createCoordinate(x, y);
            Ship *ship = getShip(board, coordinate);
            if (isTileShip(board, coordinate) && ship->type == ship_type && !isShipSunk(getShip(board, coordinate))) {
                /* Ship is still alive */
                return true;
            }
        }
    }

    /* Ship is not found or is sunk */
    return false;
}

/* Ship Placements */
void placeShip(Board *board, Ship *ship, Coordinate position) {
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

void placeMine(Board *board, Coordinate position) { setTileState(getTile(board, position), MINE); }

/* Ship Getters */
Ship *getShip(Board *board, Coordinate position) { return getTile(board, position)->ship; }

Ship *getShipWithName(Board *board, char *name) { return dictionaryGet(board->ships_by_name, name); }

/* Printing */
void printBoard(Board *board, bool showShips) {
    Ship *ship;


    /* Constants for ASCII character codes*/
    const int UPPERCASE_A = 'A';
    const int SPACE = ' ';

    /* Get the dimensions of the board*/
    const unsigned int width = board->WIDTH;
    const unsigned int height = board->HEIGHT;

    unsigned int x, y;

    /*Print the column labels*/
    printf("  ");
    for (x = 0; x < width; ++x) {
        printf("%d ", x);
    }
    printf("\n");

    /* Iterate through each tile in the board*/
    for (y = 0; y < height; ++y) {
        /* Print the row label*/
        printf("%c ", UPPERCASE_A + y);

        for (x = 0; x < width; ++x) {
            /* Get the state of the current tile*/
            State state = board->tiles[x + y * width].state;

            /* Print the appropriate character for the tile state*/
            switch (state) {
                case WATER:
                PRINT_BLUE_TEXT("~ ");
                    break;
                case SHIP:
                    /* Get the ship that occupies the tile*/
                    ship = getShip(board, createCoordinate(x, y));

                    /* Check if the ship has been completely sunk*/
                    if (ship->hits == (1 << ship->type) - 1) {
                        PRINT_CYAN_TEXT("□ ");
                    } else {
                        /* Check if the current tile has been hit*/
                        if (isShipHitAtCoordinate(ship, createCoordinate(x, y))) {
                            PRINT_RED_TEXT("□ ");
                        } else if (showShips) {
                            /* Print a square if the ship is not sunk and showShips is true*/
                            printf("■ ");
                        } else {
                            /* Print water if the ship is not sunk and showShips is false*/
                            PRINT_BLUE_TEXT("~ ");
                        }
                    }
                    break;
                case MINE:
                PRINT_YELLOW_TEXT("⚠ ");
                    break;
                default:
                    /* Print two spaces for other tile states*/
                    printf("%c%c", SPACE, SPACE);
                    break;
            }
        }
        printf("\n");
    }
}

void printBoardRow(Board *board, unsigned int y, bool showShips) {
    Ship *ship;
    const int SPACE = ' ';

    /* Get the dimensions of the board*/
    const unsigned int width = board->WIDTH;

    unsigned int x;


    for (x = 0; x < width; ++x) {
        /* Get the state of the current tile*/
        State state = board->tiles[x + y * width].state;

        /* Print the appropriate character for the tile state*/
        switch (state) {
            case WATER:
            PRINT_BLUE_TEXT("~  ");
                break;
            case SHIP:
                /* Get the ship that occupies the tile*/
                ship = getShip(board, createCoordinate(x, y));

                /* Check if the ship has been completely sunk*/
                if (ship->hits == (1 << ship->type) - 1) {
                    PRINT_CYAN_TEXT("□  ");
                } else {
                    /* Check if the current tile has been hit*/
                    if (isShipHitAtCoordinate(ship, createCoordinate(x, y))) {
                        PRINT_RED_TEXT("□  ");
                    } else if (showShips) {
                        /* Print a square if the ship is not sunk and showShips is true*/
                        printf("■  ");
                    } else {
                        /* Print water if the ship is not sunk and showShips is false*/
                        PRINT_BLUE_TEXT("~  ");
                    }
                }
                break;
            case MINE:
            PRINT_YELLOW_TEXT("⚠  ");
                break;
            default:
                /* Print two spaces for other tile states*/
                printf("%c%c%c", SPACE, SPACE, SPACE);
                break;
        }
    }
}


/* Movements */
void moveShip(Board *board, Ship *ship, Direction direction) {
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
bool handleCollision(Board *board, Ship *ship, Coordinate position) {
    unsigned int i;
    Coordinate current;
    bool collision = false;

    for (i = 0; i < ship->type; ++i) {
        current.x = position.x + (ship->orientation == HORIZONTAL ? i : 0);
        current.y = position.y + (ship->orientation == VERTICAL ? i : 0);

        if (getTileState(board, current) == SHIP) {
            Ship *otherShip = getShip(board, current);
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

/* Fire */
void fireAt(Board *board, Coordinate position) {
    Coordinate head;
    Ship *ship;

    if (isCoordinateValid(board, position) == false) {
        RAISE_ERROR(ERR_INVALID_COORDINATES);
    }

    if (getTileState(board, position) == MINE) {
        printf("You already fired at this position! (%u, %u)\n", position.x, position.y);
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
