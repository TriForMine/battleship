#ifndef BATTLESHIP_GAME_H
#define BATTLESHIP_GAME_H

#include <stdbool.h>
#include "board.h"
#include "../utils/error.h"
#include "../utils/random.h"
#include "ship.h"
#include "../utils/types.h"
#include "../utils/stdprof.h"

/* Game */
Game* createGame(bool single_board, AI_Mode ai_mode, unsigned int WIDTH, unsigned int HEIGHT);
void printGame(Game* game);
void resetGame(Game* game);
void freeGame(Game* game);

/* Victory */
bool checkVictory(Game* game);

/* Turn Management */
void setGameTurn(Game* game, GameTurn turn);
void setGameTurnToPlayer1(Game* game);
void setGameTurnToPlayer2(Game* game);
void setGameTurnToOpponent(Game* game);
Board* getCurrentPlayerBoard(Game* game);
Board* getOtherPlayerBoard(Game* game);
Board* getPlayerBoard(Game* game, char player);

/* Actions */
void fire(Game* game, Coordinate coordinate);
void move(Game* game, Ship* ship, Direction direction);

#endif
