#include "board.h"
#include "ship.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    Board board;
    initBoard(&board, 10, 10);

    Ship ship;
    initShip(&ship, 5, HORIZONTAL);
    placeShip(&board, &ship, (Coordinate) { 3, 4 });

    Ship ship2;
    initShip(&ship2, 2, VERTICAL);
    placeShip(&board, &ship2, (Coordinate) { 1, 4 });

    fire(&board, (Coordinate) { 3, 4 });

    printBoard(&board);

    freeBoard(&board);

    return 0;
}
