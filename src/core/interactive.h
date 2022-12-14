#ifndef BATTLESHIP_INTERACTIVE_H
#define BATTLESHIP_INTERACTIVE_H

#include <ctype.h>
#include "../ai/common.h"
#include "../ai/random_ai.h"
#include "../ai/hunt_target.h"
#include "../ai/probability_targeting.h"
#include "../game/game.h"
#include "../multiplayer/socket.h"
#include "../utils/const.h"
#include "../utils/error.h"
#include "../utils/stdprof.h"
#include "../utils/types.h"
#include "../utils/version.h"

/* Main */
Game *startGame(int shipLengths[], int shipLengthsLengths);

Game *startOnlineGame(int *sockId, int *onlinePlayerId, int shipLengths[], int shipLengthsLengths);

void handleInteractiveGame(void);

/* AI */
void performAiTurn(Game *game);

/* Player */
bool parseLine(Game *game, char *line);

/* Helpers */
void parseCoordinate(const char *str, Coordinate *coordinate, unsigned int size, const int orientation,
                     unsigned int WIDTH, unsigned int HEIGHT);

#endif /* BATTLESHIP_INTERACTIVE_H */
