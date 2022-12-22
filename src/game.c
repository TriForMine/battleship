#include "game.h"
#include "board.h"

/* Game */
Game* createGame(bool single_board, AI_Mode ai_mode, unsigned int WIDTH, unsigned int HEIGHT) {
    Game* game = malloc_prof(sizeof(Game));
    game->board1 = createBoard(WIDTH, HEIGHT);
    game->board2 = createBoard(WIDTH, HEIGHT);
    game->single_board = single_board;
    game->turn = Player1;
    game->remaining_ships[0] = 0;
    game->remaining_ships[1] = 0;

    /* Initialize the random seed. */
    game->seed = generate_random_seed();
    /* Initialize the random number generator. */
    srand(game->seed);

    game->ai_mode = ai_mode;
    return game;
}

void printGame(Game* game) {
    if (game->single_board) {
        printBoard(game->board1);
    } else {
        printf("Player 1:\n");
        printBoard(game->board1);
        printf("Player 2:\n");
        printBoard(game->board2);
    }
}

void resetGame(Game* game) {
    game->turn = Player1;
    game->remaining_ships[0] = 0;
    game->remaining_ships[1] = 0;
}

void freeGame(Game* game) {
    if (game->board1 != NULL) {
        freeBoard(game->board1);
        game->board1 = NULL;
    }
    if (game->board2 != NULL) {
        freeBoard(game->board2);
        game->board2 = NULL;
    }
    if (game != NULL) {
        free_prof(game);
    }
}

/* Victory */
bool checkVictory(Game* game) { return game->remaining_ships[0] == 0 || game->remaining_ships[1] == 0; }

/* Turn Management */

void setGameTurn(Game* game, GameTurn turn) { game->turn = turn; }

void setGameTurnToPlayer1(Game* game) { setGameTurn(game, Player1); }

void setGameTurnToPlayer2(Game* game) { setGameTurn(game, Player2); }

void setGameTurnToOpponent(Game* game) { setGameTurn(game, game->turn == Player1 ? Player2 : Player1); }

Board* getCurrentPlayerBoard(Game* game) {
    return game->single_board || game->turn == Player1 ? game->board1 : game->board2;
}

Board* getOtherPlayerBoard(Game* game) {
    return game->single_board || game->turn == Player2 ? game->board1 : game->board2;
}

Board* getPlayerBoard(Game* game, char player) {
    return game->single_board == true || player == '1' ? game->board1 : game->board2;
}

/* Actions */
void fire(Game* game, Coordinate coordinate) {
    Ship* ship;
    Board* board = getOtherPlayerBoard(game);

    if (!isCoordinateValid(board, coordinate)) {
        RAISE_ERROR(ERR_INVALID_COORDINATES);
    }

    fireAt(board, coordinate);
    setGameTurnToOpponent(game);

    ship = getShip(board, coordinate);

    if (ship != NULL) {
        if (isShipSunk(ship)) {
            game->remaining_ships[game->turn == Player1 ? 1 : 0]--;
        }
    }
}

void move(Game* game, Ship* ship, Direction direction) {
    Board* board = getCurrentPlayerBoard(game);
    moveShip(board, ship, direction);
    setGameTurnToOpponent(game);

    if (ship != NULL) {
        if (isShipSunk(ship)) {
            game->remaining_ships[game->turn == Player1 ? 1 : 0]--;
        }
    }
}
