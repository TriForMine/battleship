#include "probability_targeting.h"

#ifdef _WIN32
#include <windows.h>
#else

#include <pthread.h>

#endif

/* Main */
void playProbabilityTargetingAI(Game *game) {
    Coordinate coordinate = getProbabilityTarget(game);

    fire(game, coordinate);
}

/* Helpers */

/* Goes through the board and checks if there are any tiles that are hit but not sunk */
bool isInTargetingMode(Board *board) {
    unsigned int x, y;

    for (x = 0; x < board->WIDTH; x++) {
        for (y = 0; y < board->HEIGHT; y++) {
            Coordinate coordinate = createCoordinate(x, y);

            if (isTileHit(board, coordinate)) {
                return true;
            }
        }
    }
    return false;
}

Coordinate getProbabilityTarget(Game *game) {
    Board *board = getOtherPlayerBoard(game);
    unsigned int *probability_density;
    Coordinate coordinate;
#if defined DEBUG_MODE && defined DISABLE_MULTITHREADING
    unsigned int x, y, i;
    unsigned int MAX_DENSITY = 1;
#endif

    probability_density = calculateProbabilityDensity(board);
    coordinate = getHighestTargetCoordinate(board, probability_density);

#if defined DEBUG_MODE && defined DISABLE_MULTITHREADING
    for (i = 0; i < board->HEIGHT * board->WIDTH; i++) {
        if (probability_density[i] > MAX_DENSITY) {
            MAX_DENSITY = probability_density[i];
        }
    }

    for (y = 0; y < board->HEIGHT; y++) {
        for (x = 0; x < board->WIDTH; x++) {
            unsigned int intensity = 255 - probability_density[y * board->WIDTH + x] * 255 / MAX_DENSITY;

            printf("\x1b[48;2;%d;%d;%dm  ", intensity, intensity, intensity);
        }
        printf("\x1b[0m");
        printf("\n");
    }
#endif

    free_prof(probability_density);

    return coordinate;
}

/* Multi-threading */
typedef struct {
    Board *board;
    bool targeting_mode;
    Ship_Type ship_type;
} ProbabilityCalcArgs;

#ifdef _WIN32
DWORD WINAPI calcProbability(LPVOID lpParam) {
#else

void *calcProbability(void *args) {
#endif
    ProbabilityCalcArgs *calc_args;
    Board *board;
    Ship_Type ship_type;
    unsigned int x, y;
    unsigned int *probability_density;
#ifdef _WIN32
    calc_args = (ProbabilityCalcArgs *) lpParam;
#else
    calc_args = (ProbabilityCalcArgs *) args;
#endif
    board = calc_args->board;
    ship_type = calc_args->ship_type;
    probability_density = (unsigned int *) malloc(
            (unsigned long) board->WIDTH * (unsigned long) board->HEIGHT * sizeof(unsigned int));
    memset(probability_density, 0, (unsigned long) board->WIDTH * (unsigned long) board->HEIGHT * sizeof(unsigned int));

    for (y = 0; y < board->HEIGHT; ++y) {
        for (x = 0; x < board->WIDTH; ++x) {
            Coordinate coordinate = createCoordinate(x, y);
            if (isTileUnknown(board, coordinate)) {
                probability_density[y * board->WIDTH + x] +=
                        getShipProbability(calc_args->board, coordinate, ship_type, calc_args->targeting_mode);
            }
        }
    }

    return probability_density;
}

/* Probability Calculation */

/* Check if the placement is valid, and count hit if in targeting mode */
bool checkPlacement(Board *board, unsigned int ship_length, bool targetMode, long fixed_coordinate,
                    long variable_coordinate, char *hit_count, bool is_x_fixed) {
    long i;
    Coordinate current_coordinate;
    bool is_valid = true;
    (*hit_count) = 0;

    for (i = 0; i < (long) ship_length; ++i) {
        if (is_x_fixed) {
            current_coordinate = createCoordinate(fixed_coordinate, variable_coordinate + i);
        } else {
            current_coordinate = createCoordinate(variable_coordinate + i, fixed_coordinate);
        }

        if (!isCoordinateValid(board, current_coordinate)) {
            is_valid = false;
            break;
        }

        if (isTileHit(board, current_coordinate)) {
            (*hit_count)++;
        }

        if (!targetMode && !isTileUnknown(board, current_coordinate)) {
            is_valid = false;
            break;
        }

        if (targetMode
            && (isTileMine(board, current_coordinate)
                || (isTileShip(board, current_coordinate) && isShipSunk(getShip(board, current_coordinate))))) {
            is_valid = false;
            break;
        }
    }

    return is_valid;
}

/* Calculates the probability density for a given ship at a given coordinate */
unsigned int getShipProbability(Board *board, Coordinate coordinate, Ship_Type ship_type, bool isTargetingMode) {
    unsigned int probability = 0;
    long x, y;
    unsigned int ship_length = getShipLength(ship_type);

    /* Check how many times it can be placed horizontally */
    for (x = (long) coordinate.x - (long) ship_length; x <= coordinate.x; ++x) {
        char hit_count = 0;
        bool is_valid = checkPlacement(board, ship_length, isTargetingMode, coordinate.y, x, &hit_count, false);

        if (is_valid) {
            if (isTargetingMode && hit_count > 0) {
                probability += hit_count;
            } else if (!isTargetingMode && hit_count == 0) {
                probability++;
            }
        }
    }

    /* Check how many times it can be placed vertically */
    for (y = (long) coordinate.y - (long) ship_length; y <= coordinate.y; ++y) {
        char hit_count = 0;
        bool is_valid = checkPlacement(board, ship_length, isTargetingMode, coordinate.x, y, &hit_count, true);

        if (is_valid) {
            if (isTargetingMode && hit_count > 0) {
                probability += hit_count;
            } else if (!isTargetingMode && hit_count == 0) {
                probability++;
            }
        }
    }

    return probability;
}

/* Calculates the probability density for all ships */
unsigned int *calculateProbabilityDensity(Board *board) {
#ifndef DISABLE_MULTITHREADING
    int i;
    unsigned int j;
#ifdef _WIN32
    HANDLE threads[4];
#else
    pthread_t threads[4];
#endif
    ProbabilityCalcArgs args[4];
#else
    unsigned int x, y;
#endif
    bool ships_alive[4];
    bool isTargetingMode = isInTargetingMode(board);
    unsigned int *probability_density = (unsigned int *) malloc(
            (unsigned long) board->WIDTH * (unsigned long) board->HEIGHT * sizeof(unsigned int));
    memset(probability_density, 0, (unsigned long) board->WIDTH * (unsigned long) board->HEIGHT * sizeof(unsigned int));

    ships_alive[3] = isShipAlive(board, CARRIER);
    ships_alive[2] = isShipAlive(board, BATTLESHIP);
    ships_alive[1] = isShipAlive(board, CRUISER);
    ships_alive[0] = isShipAlive(board, DESTROYER);

#ifndef DISABLE_MULTITHREADING
    /* Set up arguments for each thread */
    for (i = 0; i < 4; i++) {
        if (ships_alive[i] == true) {
            args[i].board = board;
            args[i].ship_type = (Ship_Type) (i + 2);
            args[i].targeting_mode = isTargetingMode;
        }
    }

    /* Create threads for each ship type */
    for (i = 0; i < 4; i++) {
        if (ships_alive[i]) {
#ifdef _WIN32
            threads[i] = CreateThread(NULL, 0, calcProbability, &args[i], 0, NULL);
#else
            pthread_create(&threads[i], NULL, calcProbability, &args[i]);
#endif
        }
    }

    /* Wait for all threads to finish and sum the results */
    for (i = 0; i < 4; i++) {
        if (ships_alive[i]) {
            void *thread_result;
            unsigned int *thread_probability_density;
#ifdef _WIN32
            WaitForSingleObject(threads[i], INFINITE);
            GetExitCodeThread(threads[i], (LPDWORD)&thread_result);
#else
            pthread_join(threads[i], &thread_result);
#endif

            thread_probability_density = (unsigned int *) thread_result;
            for (j = 0; j < board->WIDTH * board->HEIGHT; j++) {
                probability_density[j] += thread_probability_density[j];
            }

            free(thread_probability_density);
        }
    }

#else
    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            Coordinate coordinate = createCoordinate(x, y);
            if (isTileUnknown(board, coordinate)) {
                if (ships_alive[3]) {
                    probability_density[y * board->WIDTH + x] +=
                            getShipProbability(board, coordinate, CARRIER, isTargetingMode);
                }
                if (ships_alive[2]) {
                    probability_density[y * board->WIDTH + x] +=
                            getShipProbability(board, coordinate, BATTLESHIP, isTargetingMode);
                }
                if (ships_alive[1]) {
                    probability_density[y * board->WIDTH + x] +=
                            getShipProbability(board, coordinate, CRUISER, isTargetingMode);
                }
                if (ships_alive[0]) {
                    probability_density[y * board->WIDTH + x] +=
                            getShipProbability(board, coordinate, DESTROYER, isTargetingMode);
                }
            }
        }
    }
#endif

    return probability_density;
}

/* Targeting */

/* Gets the coordinate with the highest probability density */
Coordinate getHighestTargetCoordinate(Board *board, const unsigned int *probability_density) {
    unsigned int x, y;
    unsigned int max = 0;
    Coordinate coordinate = createCoordinate(0, 0);

    for (x = 0; x < board->WIDTH; x++) {
        for (y = 0; y < board->HEIGHT; y++) {
            if (probability_density[y * board->WIDTH + x] > max) {
                max = probability_density[y * board->WIDTH + x];
                coordinate = createCoordinate(x, y);
            }
        }
    }
    return coordinate;
}