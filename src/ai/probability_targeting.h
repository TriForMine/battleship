#ifndef BATTLESHIP_PROBABILITY_TARGETING_H
#define BATTLESHIP_PROBABILITY_TARGETING_H

#include <stdbool.h>
#include "../game/game.h"
#include "../utils/helpers.h"
#include <math.h>
/* Main */
void playProbabilityTargetingAI(Game* game);

/* Helpers */
bool isInTargetingMode(Board* board);

/* Probability Calculation */
unsigned int getShipProbability(Board* board, Coordinate coordinate, Ship_Type ship_type);
unsigned int* calculateProbabilityDensity(Board* board);

/* Targeting */
Coordinate getTargetCoordinate(Board* board, const unsigned int* probability_density);

#endif /* BATTLESHIP_PROBABILITY_TARGETING_H */