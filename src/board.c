#include "board.h"
#include "const.h"
#include "tile.h"
#include <stdio.h>
#include <stdlib.h>

/* Board */
Board*
createBoard(int WIDTH, int height) {
    Board* board = malloc(sizeof(Board));
    int x, y;

    board->WIDTH = WIDTH;
    board->HEIGHT = height;
    board->tiles = malloc(sizeof(Tile) * board->WIDTH * board->HEIGHT);
    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            initTile(&board->tiles[x + y * board->WIDTH]);
        }
    }

    return board;
}

void
resetBoard(Board* board) {
    int x, y;

    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            resetTile(&board->tiles[x + y * board->WIDTH]);
        }
    }
}

void
freeBoard(Board* board) {
    int x, y;

    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            freeTile(board, &board->tiles[x + y * board->WIDTH]);
        }
    }
    if (board->tiles != NULL) {
        free(board->tiles);
    }

    free(board);
}

Tile
getTile(Board* board, Coordinate coordinate) {
    return board->tiles[coordinate.x + coordinate.y * board->WIDTH];
}

State
getTileState(Board* board, Coordinate coordinate) {
    return getTile(board, coordinate).state;
}

/* Ship Placements */
void
placeShip(Board* board, Ship* ship, Coordinate position) {
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

    ship->head = position;
}

void
placeMine(Board* board, Coordinate position) {
    setTileState(&board->tiles[position.x + position.y * board->WIDTH], MINE);
}

/* Ship Getters */
Ship*
getShip(Board* board, Coordinate position) {
    return board->tiles[position.x + position.y * board->WIDTH].ship;
}

/* Printing */
void
printBoard(Board* board) {

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
void
moveShip(Board* board, Ship* ship, Direction direction) {
    int i;
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
                for (i = 0; i < ship->type; ++i) {
                    current.x = position.x + i;
                    current.y = position.y;

                    if (getTileState(board, current) == MINE) {
                        ship->hits |= 1 << i;
                    }

                    removeTileShip(&board->tiles[current.x + ship->head.y * board->WIDTH]);
                    setTileShip(&board->tiles[current.x + current.y * board->WIDTH], ship);
                }
                break;
            case LEFT:
                if (getTileState(board, position) == MINE) {
                    ship->hits |= 1 << 0;
                }

                removeTileShip(&board->tiles[(ship->head.x + ship->type - 1) + ship->head.y * board->WIDTH]);
                setTileShip(&board->tiles[position.x + position.y * board->WIDTH], ship);
                break;
            case RIGHT:
                if (getTileState(board, createCoordinate((ship->head.x + ship->type), ship->head.y)) == MINE) {
                    ship->hits |= 1 << (ship->type - 1);
                }

                removeTileShip(&board->tiles[ship->head.x + ship->head.y * board->WIDTH]);
                setTileShip(&board->tiles[(ship->head.x + ship->type) + position.y * board->WIDTH], ship);
                break;
        }
    } else {
        switch (direction) {
            case UP:
                if (getTileState(board, position) == MINE) {
                    ship->hits |= 1 << 0;
                }

                removeTileShip(&board->tiles[ship->head.x + (ship->head.y + ship->type - 1) * board->WIDTH]);
                setTileShip(&board->tiles[position.x + position.y * board->WIDTH], ship);
                break;
            case DOWN:
                if (getTileState(board, createCoordinate(ship->head.x, ship->head.y + ship->type)) == MINE) {
                    ship->hits |= 1 << (ship->type - 1);
                }

                removeTileShip(&board->tiles[ship->head.x + ship->head.y * board->WIDTH]);
                setTileShip(&board->tiles[position.x + (position.y + ship->type - 1) * board->WIDTH], ship);
                break;
            case LEFT:
            case RIGHT:
                for (i = 0; i < ship->type; ++i) {
                    current.x = position.x;
                    current.y = position.y + i;

                    if (getTileState(board, current) == MINE) {
                        ship->hits |= 1 << i;
                    }

                    removeTileShip(&board->tiles[ship->head.x + (ship->head.y + i) * board->WIDTH]);
                    setTileShip(&board->tiles[current.x + current.y * board->WIDTH], ship);
                }
                break;
        }
    }

    ship->head = position;
}

void
moveUp(Board* board, Coordinate position) {
    int i;
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.y > 0) {
            moveShip(board, ship, UP);
        }
    }
}

void
moveDown(Board* board, Coordinate position) {
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.y < board->HEIGHT - 1) {
            moveShip(board, ship, DOWN);
        }
    }
}

void
moveLeft(Board* board, Coordinate position) {
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.x > 0) {
            moveShip(board, ship, LEFT);
        }
    }
}

void
moveRight(Board* board, Coordinate position) {
    Ship* ship = getShip(board, position);
    if (ship != NULL) {
        if (position.x < board->WIDTH - 1) {
            moveShip(board, ship, RIGHT);
        }
    }
}

/* Fire */
void
fire(Board* board, Coordinate position) {
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
