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

            if (isTileHit(board, coordinate) == true) {
                return true;
            }
        }
    }
    return false;
}

/* Probability Calculation */

/* Calculates the probability density for a given ship at a given coordinate */
unsigned int getShipProbability(Board* board, Coordinate coordinate, Ship_Type ship_type) {
    unsigned int probability = 0;
    long x, x2, y, y2;
    unsigned int ship_length = getShipLength(ship_type);
    bool targetMode = isInTargetingMode(board);

    /* Check how many times it can be placed horizontally */
    for (x = (long)coordinate.x - (long)ship_length; x <= coordinate.x; ++x) {
        Coordinate current_coordinate = createCoordinate(x, coordinate.y);
        bool is_valid = true;
        bool has_hit = false;

        for (x2 = x; x2 < x + (long)ship_length; ++x2) {
            current_coordinate.x = x2;

            if (isCoordinateValid(board, current_coordinate) == false) {
                is_valid = false;
                break;
            }

            if (has_hit == false) {
                has_hit = isTileHit(board, current_coordinate);
            }
            if (targetMode == false && isTileUnknown(board, current_coordinate) == false) {
                is_valid = false;
                break;
            }
        }

        if (is_valid == true) {
            if (targetMode == true && has_hit == true) {
                probability++;
            } else if (targetMode == false && has_hit == false) {
                probability++;
            }
        }
    }

    /* Check how many times it can be placed vertically */
    for (y = (long)coordinate.y - (long)ship_length; y <= coordinate.y; ++y) {
        Coordinate current_coordinate = createCoordinate(coordinate.x, y);
        bool is_valid = true;
        bool has_hit = false;

        for (y2 = y; y2 < y + (long)ship_length; ++y2) {
            current_coordinate.y = y2;
            if (isCoordinateValid(board, current_coordinate) == false) {
                is_valid = false;
                break;
            }

            if (has_hit == false) {
                has_hit = isTileHit(board, current_coordinate);
            }
            if (targetMode == false && isTileUnknown(board, current_coordinate) == false) {
                is_valid = false;
                break;
            }
        }

        if (is_valid == true) {
            if (targetMode == true && has_hit == true) {
                probability++;
            } else if (targetMode == false && has_hit == false) {
                probability++;
            }
        }
    }

    return probability;
}

/* Calculates the probability density for all ships */
unsigned int* calculateProbabilityDensity(Board* board) {
    unsigned int x, y;
    unsigned int* probability_density = calloc_prof(board->WIDTH * board->HEIGHT, sizeof(unsigned int));

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
