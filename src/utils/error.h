#ifndef ERROR_H
#define ERROR_H

#define ERR_INVALID_BOARD_DIMENSIONS      (-1)
#define ERR_INVALID_SHIP_TYPE             (-2)
#define ERR_INVALID_SHIP_POSITION         (-3)
#define ERR_SHIP_OVERLAP                  (-4)
#define ERR_INVALID_COORDINATES           (-5)
#define ERR_COORDINATE_ALREADY_HIT        (-6)
#define ERR_GAME_ALREADY_OVER             (-7)
#define ERR_INVALID_PLAYER                (-8)
#define ERR_INVALID_TURN_PLAYER           (-9)
#define ERR_CANNOT_PLACE_SHIP_AFTER_START (-10)
#define ERR_INVALID_SHIP_DIRECTION        (-11)
#define ERR_SUNK_SHIP                     (-12)
#define ERR_INVALID_SHIP_PLACEMENT        (-13)
#define ERR_UNKNOWN_COMMAND               (-14)
#define ERR_OPENING_FILE                  (-15)
#define ERR_INVALID_SHIP                  (-16)

extern const char* error_messages[];

#define RAISE_ERROR(err)                                                                                               \
    do {                                                                                                               \
        if ((err) < 0) {                                                                                               \
            fprintf(stderr, "Error: %s\n", error_messages[-(err)]);                                                    \
            exit(err);                                                                                                 \
        }                                                                                                              \
    } while (0)

#define RAISE_ERROR_WITH_LINE(err, line)                                                                               \
    do {                                                                                                               \
        if ((err) < 0) {                                                                                               \
            fprintf(stderr, "Error: %s (Line %d)\n", error_messages[-(err)], line);                                    \
            exit(err);                                                                                                 \
        }                                                                                                              \
    } while (0)

#endif