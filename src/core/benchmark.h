#ifndef BATTLESHIP_BENCHMARK_H
#define BATTLESHIP_BENCHMARK_H

#include "../game/game.h"
#include "../ai/random_ai.h"
#include "../ai/hunt_target.h"
#include "../ai/probability_targeting.h"
#include "../ai/common.h"
#include "time.h"

/* Main */
void startBenchmark(unsigned int num_games);

/* Helpers */
typedef struct {
    double time;
    unsigned int minimum_moves;
    unsigned int maximum_moves;
    float average_moves;
    unsigned int median_moves;
    unsigned int num_games;
    unsigned int *moves;
} BenchmarkResult;

BenchmarkResult benchmarkStrategy(Game *game, Coordinate (*getTargetCoordinate)(Game *), unsigned int num_games);

unsigned int playGame(Game *game, Coordinate (*getTargetCoordinate)(Game *));

void printGraph(BenchmarkResult result);

#endif /* BATTLESHIP_BENCHMARK_H */