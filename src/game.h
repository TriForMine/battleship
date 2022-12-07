#ifndef BATTLESHIP_GAME_H
#define BATTLESHIP_GAME_H

#include "types.h"

/* Game */
void initGame(Board* board1, Board* board2);
void freeGame(Board* board1, Board* board2);
void resetGame(Board* board1, Board* board2);

/* Turn Management */
void setGameTurn(GameTurn* turn);
void setGameTurnToPlayer1(GameTurn* turn);
void setGameTurnToPlayer2(GameTurn* turn);
void setGameTurnToOpponent(GameTurn* turn);

/* Actions */
void fire(Game* game, Coordinate* coordinate);
void move(Game* game, Coordinate* coordinate, Direction* direction);

#endif
