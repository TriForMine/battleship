#include "random_ai.h"

void playRandomAI(Game* game) {
    Coordinate coordinate;
    Board* board = getOtherPlayerBoard(game);
    do {
        coordinate.x = rand() % board->WIDTH;
        coordinate.y = rand() % board->HEIGHT;
    } while (game->state == PLAYING && getTileState(board, coordinate) == MINE);
    fire(game, coordinate);
}
