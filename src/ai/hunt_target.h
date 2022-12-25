#ifndef BATTLESHIP_HUNT_TARGET_H
#define BATTLESHIP_HUNT_TARGET_H

#include "../utils/stdprof.h"
#include "../game/game.h"
#include "../utils/types.h"

/* Main */
void playHuntTargetAI(Game* game);

/* Helpers */
void getTargetList(Board* board, Coordinate* result, int* size);
void getPriorityTargetList(Board* board, Coordinate* targetList, int targetListSize, int* size);
Coordinate getRandomTarget(Board* board, Coordinate* targetList, int size);
Coordinate getNextTarget(Board* board);
Coordinate getHuntTargetCoordinate(Game* game);

#endif /* BATTLESHIP_HUNT_TARGET_H */
