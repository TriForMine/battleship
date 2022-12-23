#ifndef BATTLESHIP_INTERACTIVE_H
#define BATTLESHIP_INTERACTIVE_H

#include "../ai/common.h"
#include "../ai/random_ai.h"
#include "../game/game.h"
#include "../utils/const.h"
#include "../utils/error.h"
#include "../utils/stdprof.h"
#include "../utils/types.h"

/* Main */
Game* startGame(int shipLengths[], int shipLengthsLengths);
void handleInteractiveGame(void);

/* AI */
void performAiTurn(Game* game);

/* Player */
bool parseLine(Game* game, char* line);

/* Helpers */
void parseCoordinate(const char* str, Coordinate* coordinate);

#endif /* BATTLESHIP_INTERACTIVE_H */
