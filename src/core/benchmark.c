#include "benchmark.h"

/* Main */
void startBenchmark(unsigned int num_games) {
    BenchmarkResult randomStrategy;
    BenchmarkResult huntTargetStrategy;
    BenchmarkResult probabilityTargetStrategy;
    Game *game = createGame(true, DISABLED, generate_random_seed(), 10, 10);
    game->state = PLAYING;

    printf("Benchmarking strategies...\n");
    randomStrategy = benchmarkStrategy(game, getRandomTargetCoordinate, num_games);
    huntTargetStrategy = benchmarkStrategy(game, getHuntTargetCoordinate, num_games);
    probabilityTargetStrategy = benchmarkStrategy(game, getProbabilityTarget, num_games);
    clearConsole();

    printf("Benchmark results:\n\n");
    PRINT_GREEN_TEXT("Random strategy:\n");
    printf("Time: %.2f seconds\n", randomStrategy.time);
    printf("Minimum moves: %u moves\n", randomStrategy.minimum_moves);
    printf("Maximum moves: %u moves\n", randomStrategy.maximum_moves);
    printf("Average moves: %.0f moves\n", randomStrategy.average_moves);
    printf("Median moves: %u moves\n\n", randomStrategy.median_moves);

    PRINT_GREEN_TEXT("\nHunt target strategy:\n");
    printf("Time: %.2f seconds\n", huntTargetStrategy.time);
    printf("Minimum moves: %u moves\n", huntTargetStrategy.minimum_moves);
    printf("Maximum moves: %u moves\n", huntTargetStrategy.maximum_moves);
    printf("Average moves: %.0f moves\n", huntTargetStrategy.average_moves);
    printf("Median moves: %u moves\n\n", huntTargetStrategy.median_moves);

    PRINT_GREEN_TEXT("\nProbability target strategy:\n");
    printf("Time: %.2f seconds\n", probabilityTargetStrategy.time);
    printf("Minimum moves: %u moves\n", probabilityTargetStrategy.minimum_moves);
    printf("Maximum moves: %u moves\n", probabilityTargetStrategy.maximum_moves);
    printf("Average moves: %.0f moves\n", probabilityTargetStrategy.average_moves);
    printf("Median moves: %u moves\n\n", probabilityTargetStrategy.median_moves);

    free_prof(randomStrategy.moves);
    free_prof(huntTargetStrategy.moves);
    free_prof(probabilityTargetStrategy.moves);

    freeGame(game);
}

/* Helpers */
BenchmarkResult benchmarkStrategy(Game *game, Coordinate (*getTargetCoordinate)(Game *), unsigned int num_games) {
    int shipLengths[] = {5, 4, 3, 3, 2};
    int shipLengthsLengths = 5;
    unsigned int i, j, percentage;
    unsigned int moves = 0;
    unsigned int min_move = 0;
    unsigned int max_move = 0;
    unsigned int *all_moves;
    clock_t start_time, end_time;
    double elapsed_time;
    BenchmarkResult result;

    all_moves = malloc(sizeof(unsigned int) * num_games);

    start_time = clock();

    resetGame(game, generate_random_seed());
    placeRandomShips(getPlayerBoard(game, Player1), shipLengths, shipLengthsLengths);

    /* Play the specified number of games */
    for (i = 0; i < num_games; i++) {
        unsigned int game_move;
        int step;
        step = 25;/*100 = 1%   50=2%   25=4%   10=10%*/
        if (i % (num_games / step) == 0) {
            clearConsole();
            percentage = i / (num_games / 100);
            printf("[");
            for (j = 0; j < percentage; j++) {
                printf("#");
            }
            for (j = percentage; j < 100; j++) {
                printf("-");
            }
            printf("] %d%%\n", percentage);
        }


        game_move = playGame(game, getTargetCoordinate);

        if (i == 0) {
            min_move = game_move;
            max_move = game_move;
        } else {
            if (game_move < min_move) {
                min_move = game_move;
            }
            if (game_move > max_move) {
                max_move = game_move;
            }
        }

        all_moves[i] = game_move;

        moves += game_move;

        /* Reset the game state for the next game */
        resetGame(game, generate_random_seed());
        placeRandomShips(getPlayerBoard(game, Player1), shipLengths, shipLengthsLengths);
    }

    end_time = clock();
    elapsed_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;

    result.time = elapsed_time;
    result.average_moves = (float) moves / (float) num_games;
    result.minimum_moves = min_move;
    result.maximum_moves = max_move;
    result.median_moves = all_moves[num_games / 2];
    result.num_games = num_games;
    result.moves = all_moves;
    return result;
}

unsigned int playGame(Game *game, Coordinate (*getTargetCoordinate)(Game *)) {
    unsigned int moves = 0;

    while (!checkVictory(game)) {
        /* Get the target coordinate using the specified targeting strategy*/
        Coordinate target_coordinate = getTargetCoordinate(game);

        /* Fire at the target coordinate and update the game state*/
        fire(game, target_coordinate);

        moves++;
    }

    /* Return the number of moves made in the game*/
    return moves;
}