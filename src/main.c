#include "board.h"
#include "helpers.h"
#include "ship.h"

#include <stdio.h>
#include <stdlib.h>

int
main(void) {
    Coordinate coord;
    Ship* ship;
    Ship* ship2;
    int i;

    Board* board = createBoard(10, 10);

    ship = createShip(CARRIER, HORIZONTAL);
    placeShip(board, ship, createCoordinate(3, 4));

    ship2 = createShip(CRUISER, VERTICAL);
    placeShip(board, ship2, createCoordinate(1, 4));

    fire(board, createCoordinate(0, 0));
    for (i = 4; i < 7; ++i) {
        fire(board, createCoordinate(2, i));
    }
    for (i = 4; i < 7; ++i) {
        fire(board, createCoordinate(0, i));
    }
    fire(board, createCoordinate(1, 3));
    fire(board, createCoordinate(1, 7));

    printBoard(board);

    printf("\n\n");

    moveDown(board, createCoordinate(1, 4));
    moveRight(board, createCoordinate(1, 5));

    printBoard(board);

    freeBoard(board);

    return 0;
}
