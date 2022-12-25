#ifndef BATTLESHIP_RANDOM_AI_H
#define BATTLESHIP_RANDOM_AI_H

#include "../game/game.h"

/* Main */
void playRandomAI(Game* game);

/* Helpers */
Coordinate getRandomTargetCoordinate(Game* game);

#endif /* BATTLESHIP_RANDOM_AI_H */
