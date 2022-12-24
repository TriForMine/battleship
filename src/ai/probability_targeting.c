#include "probability_targeting.h"

/* Main */
void playProbabilityTargetingAI(Game* game) {
    Board* board = getOtherPlayerBoard(game);
    unsigned int* probability_density = calculateProbabilityDensity(getOtherPlayerBoard(game));
    Coordinate coordinate = getTargetCoordinate(board, probability_density);

#ifdef DEBUG_MODE
    unsigned int x, y, i;
    unsigned int MAX_DENSITY = 1;
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
    fire(game, coordinate);
}

/* Helpers */

/* Goes through the board and checks if there are any tiles that are hit but not sunk */
bool isInTargetingMode(Board* board) {
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

/* Probability Calculation */

/* Check if the placement is valid, and count hit if in targeting mode */
bool checkPlacement(Board* board, unsigned int ship_length, bool targetMode, long fixed_coordinate,
                    long variable_coordinate, char* hit_count, bool is_x_fixed) {
    long i;
    Coordinate current_coordinate;
    bool is_valid = true;
    (*hit_count) = 0;

    for (i = 0; i < (long)ship_length; ++i) {
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
unsigned int getShipProbability(Board* board, Coordinate coordinate, Ship_Type ship_type) {
    unsigned int probability = 0;
    long x, y;
    unsigned int ship_length = getShipLength(ship_type);
    bool targetMode = isInTargetingMode(board);

    /* Check how many times it can be placed horizontally */
    for (x = (long)coordinate.x - (long)ship_length; x <= coordinate.x; ++x) {
        char hit_count = 0;
        bool is_valid = checkPlacement(board, ship_length, targetMode, coordinate.y, x, &hit_count, false);

        if (is_valid) {
            if (targetMode && hit_count > 0) {
                probability += hit_count;
            } else if (!targetMode && hit_count == 0) {
                probability++;
            }
        }
    }

    /* Check how many times it can be placed vertically */
    for (y = (long)coordinate.y - (long)ship_length; y <= coordinate.y; ++y) {
        char hit_count = 0;
        bool is_valid = checkPlacement(board, ship_length, targetMode, coordinate.x, y, &hit_count, true);

        if (is_valid) {
            if (targetMode && hit_count > 0) {
                probability += hit_count;
            } else if (!targetMode && hit_count == 0) {
                probability++;
            }
        }
    }

    return probability;
}

/* Calculates the probability density for all ships */
unsigned int* calculateProbabilityDensity(Board* board) {
    unsigned int x, y;
    unsigned int* probability_density =
        calloc_prof((unsigned long)board->WIDTH * (unsigned long)board->HEIGHT, sizeof(unsigned int));

    for (x = 0; x < board->WIDTH; ++x) {
        for (y = 0; y < board->HEIGHT; ++y) {
            Coordinate coordinate = createCoordinate(x, y);
            if (isTileUnknown(board, coordinate)) {
                probability_density[y * board->WIDTH + x] = getShipProbability(board, coordinate, CARRIER);
                probability_density[y * board->WIDTH + x] += getShipProbability(board, coordinate, BATTLESHIP);

                /* There is 2 cruisers in the game */
                probability_density[y * board->WIDTH + x] += getShipProbability(board, coordinate, CRUISER);
                probability_density[y * board->WIDTH + x] += getShipProbability(board, coordinate, CRUISER);

                probability_density[y * board->WIDTH + x] += getShipProbability(board, coordinate, DESTROYER);
            }
        }
    }
    return probability_density;
}

/* Targeting */

/* Gets the coordinate with the highest probability density */
Coordinate getTargetCoordinate(Board* board, const unsigned int* probability_density) {
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
