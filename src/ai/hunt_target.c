#include "hunt_target.h"

void playHuntTargetAI(Game* game) {
    Coordinate coordinate = getHuntTargetCoordinate(game);
    fire(game, coordinate);
}

/* Returns all the coordinate where the AI didn't shoot yet, and only in even tiles */
void getTargetList(Board* board, Coordinate* result, int* size) {
    Coordinate current;
    State state;
    unsigned int i, j;
    int k = 0;

    for (i = 0; i < board->WIDTH; i += 1) {
        for (j = 0; j < board->HEIGHT; j += 1) {
            if ((i + j) % 2 == 0) {
                current.x = i;
                current.y = j;
                state = getTileState(board, current);
                if (state == WATER
                    || (state == SHIP && isShipHitAtCoordinate(getShip(board, current), current) == false)) {
                    result[k] = current;
                    ++k;
                }
            }
        }
    }
    
    *size = k;
}

/* Returns a list of coordinates where the AI has to shoot in priority
 * i.e: Near the already hit tiles
 * */
void getPriorityTargetList(Board* board, Coordinate* targetList, int targetListSize, int* size) {
    Coordinate* result = malloc_prof(sizeof(Coordinate) * 4);
    Coordinate current;
    State state;
    Ship* ship;
    unsigned int i, j, k, l;

    for (i = 0; i < board->WIDTH; ++i) {
        for (j = 0; j < board->HEIGHT; ++j) {
            current.x = i;
            current.y = j;
            state = getTileState(board, current);
            ship = getShip(board, current);
            if (state == SHIP && isShipHitAtCoordinate(ship, current) == true && isShipSunk(ship) == false) {
                getNearbyCoordinates(board, current, result, &k);
                if (k > 0) {
                    for (l = 0; l < k; ++l) {
                        state = getTileState(board, result[l]);
                        if (state == WATER
                            || (state == SHIP
                                && isShipHitAtCoordinate(getShip(board, result[l]), result[l]) == false)) {
                            if (*size < targetListSize) {
                                targetList[*size] = result[l];
                                ++(*size);
                            } else {
                                /* targetList is full, stop adding coordinates */
                                break;
                            }
                        }
                    }
                    if (*size > 0) {
                        break;
                    }
                }
            }
        }
    }

    if (result != NULL) {
        free_prof(result);
    }
}

/* Returns a random target from the target list */
Coordinate getRandomTarget(Board* board, Coordinate* targetList, int size) {
    Coordinate result;
    int i;
    if (size == 0) {
        do {
            result.x = rand() % board->WIDTH;
            result.y = rand() % board->HEIGHT;
        } while (getTileState(board, result) == MINE);
    } else {
        i = rand() % size;
        result = targetList[i];
    }
    return result;
}

Coordinate getNextTarget(Board* board) {
    Coordinate result;
    Coordinate* targetList = malloc_prof(sizeof(Coordinate) * board->WIDTH * board->HEIGHT);
    int size = 0;

    getPriorityTargetList(board, targetList, 4, &size);

    if (size == 0) {
        getTargetList(board, targetList, &size);
    }

    result = getRandomTarget(board, targetList, size);

    free_prof(targetList);

    return result;
}

Coordinate getHuntTargetCoordinate(Game* game) {
    Coordinate coordinate;
    Board* board = getOtherPlayerBoard(game);
    do {
        coordinate = getNextTarget(board);
    } while (game->state == PLAYING && getTileState(board, coordinate) == MINE);

    return coordinate;
}