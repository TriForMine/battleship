#include "board.h"
#include "ship.h"
#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    Coordinate coord;
    Ship* ship;
    Ship* ship2;

    Board* board = createBoard(10, 10);

    ship = createShip(CARRIER, HORIZONTAL);
    placeShip(board, ship, createCoordinate(3, 4));

    ship2 = createShip(CRUISER, VERTICAL);
    placeShip(board, ship2, createCoordinate(1, 4));

    fire(board, createCoordinate(1, 4));
    fire(board, createCoordinate(0, 0));
    fire(board, createCoordinate(3, 4));
    printBoard(board);

    printf("\n\n");
    moveRight(board, createCoordinate(4, 4));
    moveDown(board, createCoordinate(3, 4));

    printBoard(board);

    freeBoard(board);

    return 0;
}
