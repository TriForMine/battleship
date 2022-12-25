#ifndef BATTLESHIP_PROBABILITY_TARGETING_H
#define BATTLESHIP_PROBABILITY_TARGETING_H

#include <stdbool.h>
#include "../game/game.h"
#include "../utils/helpers.h"
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#include <stdint.h>
#else

#include <pthread.h>

#endif

/* Main */
void playProbabilityTargetingAI(Game *game);

/* Helpers */
bool isInTargetingMode(Board *board);

Coordinate getProbabilityTarget(Game *game);

/* Multi-threading */
#ifdef _WIN32
DWORD WINAPI calcProbability(LPVOID lpParam);
#else

void *calcProbability(void *args);

#endif

/* Probability Calculation */
bool checkPlacement(Board *board, unsigned int ship_length, bool targetMode, long fixed_coordinate,
                    long variable_coordinate, char *hit_count, bool is_x_fixed);

unsigned int getShipProbability(Board *board, Coordinate coordinate, Ship_Type ship_type, bool isTargetingMode);

unsigned int *calculateProbabilityDensity(Board *board);

/* Targeting */
Coordinate getHighestTargetCoordinate(Board *board, const unsigned int *probability_density);

#endif /* BATTLESHIP_PROBABILITY_TARGETING_H */
