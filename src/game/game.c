#include "game.h"
#include "board.h"

/* Game */
Game *createGame(bool single_board, AI_Mode ai_mode, unsigned int SEED, unsigned int WIDTH, unsigned int HEIGHT) {
    Game *game = malloc_prof(sizeof(Game));
    game->board1 = createBoard(WIDTH, HEIGHT);
    game->board2 = createBoard(WIDTH, HEIGHT);
    game->state = PLACING_SHIPS;
    game->single_board = single_board;
    game->turn = Player1;
    game->remaining_ships[0] = 5;
    game->remaining_ships[1] = 5;
    game->seed = SEED;

    /* Initialize the random number generator. */
    srand(game->seed);

    game->ai_mode = ai_mode;
    return game;
}

void printGame(Game *game) {
    unsigned int x, y, i;
    const int UPPERCASE_A = 'A';

    const char *PLAYER1 = "Player 1";
    const char *PLAYER2 = "Player 2";

    if (game->single_board) {
        printBoard(game->board1, true);
    } else {
        printf("Player %d's turn\n", game->turn == Player1 ? 1 : 2);

        for (i = 0; i < game->board1->WIDTH - strlen(PLAYER1) / 2 + 1; ++i) {
            printf(" ");
        }

        if (game->turn == Player1) {
            SET_YELLOW_TEXT();
        }
        printf("%s", PLAYER1);
        if (game->turn == Player1) {
            RESET_TEXT_COLOR();
        }
        for (i = 0; i < game->board1->WIDTH - strlen(PLAYER1) / 2 + 1; ++i) {
            printf(" ");
        }
        printf("\t\t");

        for (i = 0; i < game->board1->WIDTH - strlen(PLAYER2) / 2 + 1; ++i) {
            printf(" ");
        }

        if (game->turn == Player2) {
            SET_YELLOW_TEXT();
        }
        printf("%s\n  ", PLAYER2);
        if (game->turn == Player2) {
            RESET_TEXT_COLOR();
        }

        for (x = 0; x < game->board1->WIDTH; ++x) {
            printf("%2d ", x);
        }
        printf("\t\t  ");
        for (x = 0; x < game->board1->WIDTH; ++x) {
            printf("%2d ", x);
        }
        printf("\n");
        for (y = 0; y < game->board1->HEIGHT; ++y) {
            /* Print the row label*/
            printf("%c  ", UPPERCASE_A + y);

            printBoardRow(game->board1, y, (game->state == ENDED || game->turn == Player1));

            printf("\t\t");

            printf("%c  ", UPPERCASE_A + y);

            printBoardRow(game->board2, y, (game->state == ENDED || game->turn == Player2));
            printf("\n");
        }
    }
}

void resetGame(Game *game, unsigned int seed) {
    game->turn = Player1;
    game->remaining_ships[0] = 5;
    game->remaining_ships[1] = 5;
    game->seed = seed;
    game->state = PLACING_SHIPS;

    resetBoard(game->board1);
    resetBoard(game->board2);
}

void freeGame(Game *game) {
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
bool checkVictory(Game *game) { return game->remaining_ships[0] == 0 || game->remaining_ships[1] == 0; }

/* Turn Management */

void setGameTurn(Game *game, GameTurn turn) { game->turn = turn; }

void setGameTurnToPlayer1(Game *game) { setGameTurn(game, Player1); }

void setGameTurnToPlayer2(Game *game) { setGameTurn(game, Player2); }

void setGameTurnToOpponent(Game *game) { setGameTurn(game, game->turn == Player1 ? Player2 : Player1); }

Board *getCurrentPlayerBoard(Game *game) {
    return game->single_board || game->turn == Player1 ? game->board1 : game->board2;
}

Board *getOtherPlayerBoard(Game *game) {
    return game->single_board || game->turn == Player2 ? game->board1 : game->board2;
}

Board *getPlayerBoard(Game *game, char player) {
    return game->single_board == true || player == '1' ? game->board1 : game->board2;
}

/* Actions */
void fire(Game *game, Coordinate coordinate) {
    Ship *ship;
    Board *board = getOtherPlayerBoard(game);

    if (!isCoordinateValid(board, coordinate)) {
        RAISE_ERROR(ERR_INVALID_COORDINATES);
    }

    fireAt(board, coordinate);
    setGameTurnToOpponent(game);

    ship = getShip(board, coordinate);

    if (ship != NULL) {
        if (isShipSunk(ship)) {
            game->remaining_ships[game->single_board == true || game->turn == Player1 ? 0 : 1]--;
        }
    }
}

void move(Game *game, Ship *ship, Direction direction) {
    Board *board = getCurrentPlayerBoard(game);
    moveShip(board, ship, direction);
    setGameTurnToOpponent(game);

    if (ship != NULL) {
        if (isShipSunk(ship)) {
            game->remaining_ships[game->single_board == true || game->turn == Player1 ? 0 : 1]--;
        }
    }
}

void moveUp(Game *game, Coordinate position) {
    Board *board = getCurrentPlayerBoard(game);
    Ship *ship = getShip(board, position);

    if (ship != NULL) {
        if (position.y > 0) {
            move(game, ship, UP);
        }
    }
}

void moveDown(Game *game, Coordinate position) {
    Board *board = getCurrentPlayerBoard(game);
    Ship *ship = getShip(board, position);

    if (ship != NULL) {
        if (position.y < board->HEIGHT - 1) {
            move(game, ship, DOWN);
        }
    }
}

void moveLeft(Game *game, Coordinate position) {
    Board *board = getCurrentPlayerBoard(game);
    Ship *ship = getShip(board, position);

    if (ship != NULL) {
        if (position.x > 0) {
            move(game, ship, LEFT);
        }
    }
}

void moveRight(Game *game, Coordinate position) {
    Board *board = getCurrentPlayerBoard(game);
    Ship *ship = getShip(board, position);

    if (ship != NULL) {
        if (position.x < board->WIDTH - 1) {
            move(game, ship, RIGHT);
        }
    }
}
