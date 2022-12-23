#include "parser.h"

#define SHOW_ONE_BOARD true

void parseSTDIN(void) {
    Game* game = NULL;
    Board* board = NULL;
    char action, player, lastPlayer, target[BUFFER_SIZE], buffer[BUFFER_SIZE];
    unsigned int width, height;
    unsigned int line = 0;
    bool placing_mode = true;
    bool is_finished = false;

    /* Read the input line by line */
    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        ++line;

        if (buffer[0] == '#') {
            /* This is a comment, ignore it */
            continue;
        } else if (sscanf(buffer, "Projet %ux%u", &height, &width) == 2) {
            /* This line defines the size of the board */
            printf("Creating board of size %u x %u\n", width, height);

            if (width < 5 || height < 5 || width * height > UINT_MAX) {
                RAISE_ERROR_WITH_LINE(ERR_INVALID_BOARD_DIMENSIONS, line);
            }

            /* Create the board */
            game = createGame(SHOW_ONE_BOARD, DISABLED, 0, width, height);
        } else if (strstr(buffer, "Jouer") != NULL) {
            /* This line signals the start of the game */
            placing_mode = false;
        } else if (sscanf(buffer, "J%c %c", &player, &action) == 2) {
            board = getPlayerBoard(game, player);

            /* This line represents a player action */
            if (lastPlayer == player && !placing_mode) {
                RAISE_ERROR_WITH_LINE(ERR_INVALID_TURN_PLAYER, line);
            }

            if (player != '1' && player != '2') {
                RAISE_ERROR_WITH_LINE(ERR_INVALID_PLAYER, line);
            }

            if (is_finished) {
                printf("Player %c won!\n", player);
                RAISE_ERROR_WITH_LINE(ERR_GAME_ALREADY_OVER, line);
            }

            if (action == 'T') {
                Coordinate position;

                /* This action has a target */
                sscanf(buffer, "J%c %c %u:%u", &player, &action, &position.x, &position.y);

                if (isCoordinateValid(board, position) == false) {
                    RAISE_ERROR_WITH_LINE(ERR_INVALID_COORDINATES, line);
                }

                fire(game, position);

                is_finished = checkVictory(game);
            } else if (action == 'G' || action == 'D' || action == 'B' || action == 'H') {
                Ship* ship = NULL;
                char direction;
                char movement_direction;
                char* ship_name = malloc_prof(sizeof(char) * MAX_LENGTH_NAME);
                unsigned int pos_x, pos_y;

                if (sscanf(buffer, "J%c %c %u:%u", &player, &direction, &pos_x, &pos_y) == 4) {
                    ship = getShip(board, createCoordinate(pos_x, pos_y));
                } else if (sscanf(buffer, "J%c %c \"%[^\"]\"", &player, &direction, ship_name) == 3) {
                    ship = getShipWithName(board, ship_name);
                } else {
                    printf("Invalid move command: %s", buffer);
                    continue;
                }

                if (ship == NULL) {
                    RAISE_ERROR_WITH_LINE(ERR_INVALID_SHIP, line);
                }

                switch (direction) {
                    case 'G':
                        movement_direction = LEFT;
                        break;
                    case 'D':
                        movement_direction = RIGHT;
                        break;
                    case 'B':
                        movement_direction = DOWN;
                        break;
                    case 'H':
                        movement_direction = UP;
                        break;
                    default:
                        RAISE_ERROR_WITH_LINE(ERR_INVALID_SHIP_DIRECTION, line);
                }

                move(game, ship, movement_direction);

                free_prof(ship_name);
            } else if (action == 'P') {
                Coordinate position;
                unsigned int start_x, start_y, end_x, end_y;
                char* ship_name = malloc_prof(sizeof(char) * MAX_LENGTH_NAME);
                Ship* ship;

                if (false == placing_mode) {
                    RAISE_ERROR_WITH_LINE(ERR_CANNOT_PLACE_SHIP_AFTER_START, line);
                }

                if (sscanf(buffer, "J%c P \"%[^\"]\" %u:%u-%u", &player, ship_name, &start_x, &start_y, &end_y) == 5) {
                    /* This is a vertical ship */
                    end_x = start_x;

                    if (end_y - start_y + 1 > 5) {
                        RAISE_ERROR_WITH_LINE(ERR_INVALID_SHIP_TYPE, line);
                    }

                    ship = createShipWithName(board, end_y - start_y + 1, VERTICAL, player, ship_name);
                } else if (sscanf(buffer, "J%c P \"%[^\"]\" %u-%u:%u", &player, ship_name, &start_x, &end_x, &start_y)
                           == 5) {
                    /* This is a horizontal ship */
                    end_y = start_y;

                    if (end_x - start_x + 1 > 5) {
                        RAISE_ERROR_WITH_LINE(ERR_INVALID_SHIP_TYPE, line);
                    }

                    ship = createShipWithName(board, end_x - start_x + 1, HORIZONTAL, player, ship_name);
                } else if (sscanf(buffer, "J%c P \"%[^\"]\" %u:%u", &player, ship_name, &start_x, &start_y) == 4) {
                    /* This is a horizontal ship */
                    end_x = start_x;
                    end_y = start_y;

                    if (end_x - start_x + 1 > 5) {
                        RAISE_ERROR_WITH_LINE(ERR_INVALID_SHIP_TYPE, line);
                    }

                    ship = createShipWithName(board, 1, HORIZONTAL, player, ship_name);
                } else if (sscanf(buffer, "J%c P %u:%u-%u", &player, &start_x, &start_y, &end_y) == 4) {
                    /* This is a vertical ship */
                    end_x = start_x;

                    if (end_y - start_y + 1 > 5) {
                        RAISE_ERROR_WITH_LINE(ERR_INVALID_SHIP_TYPE, line);
                    }

                    ship = createShip(end_y - start_y + 1, VERTICAL, player);
                    free_prof(ship_name);
                } else if (sscanf(buffer, "J%c P %u-%u:%u", &player, &start_x, &end_x, &start_y) == 4) {
                    /* This is a horizontal ship */
                    end_y = start_y;

                    if (end_x - start_x + 1 > 5) {
                        RAISE_ERROR_WITH_LINE(ERR_INVALID_SHIP_TYPE, line);
                    }

                    ship = createShip(end_x - start_x + 1, HORIZONTAL, player);
                    free_prof(ship_name);
                } else if (sscanf(buffer, "J%c P %u:%u", &player, &start_x, &start_y) == 3) {
                    /* This is a horizontal ship */
                    end_x = start_x;
                    end_y = start_y;

                    if (end_x - start_x + 1 > 5) {
                        RAISE_ERROR_WITH_LINE(ERR_INVALID_SHIP_TYPE, line);
                    }

                    ship = createShip(1, HORIZONTAL, player);
                    free_prof(ship_name);
                } else {
                    RAISE_ERROR_WITH_LINE(ERR_INVALID_SHIP_PLACEMENT, line);
                }

                position = createCoordinate(start_x, start_y);

                if (isCoordinateValid(board, position) == false) {
                    RAISE_ERROR_WITH_LINE(ERR_INVALID_SHIP_PLACEMENT, line);
                }

                placeShip(board, ship, createCoordinate(start_x, start_y));
                if (player == '1') {
                    game->remaining_ships[0]++;
                } else {
                    game->remaining_ships[1]++;
                }
            } else {
                strcpy(target, "");
            }
        } else if (strstr(buffer, "Afficher") != NULL) {
            printf("\n\nShowing current state\n\n");
            printGame(game);
        } else {
            RAISE_ERROR_WITH_LINE(ERR_UNKNOWN_COMMAND, line);
        }

        lastPlayer = player;
    }

    if (is_finished) {
        printf("\n=====================\n\nPlayer %c won!\n", player);
    } else {
        printf("\n=====================\n\nGame ended without a winner\n\nShip left:\nPlayer 1: %u\nPlayer 2: %u\n",
               game->remaining_ships[0], game->remaining_ships[1]);
    }

    /* Free the board */
    if (game != NULL) {
        freeGame(game);
    }
}

void parseFile(char* filename) {
    FILE* fp = freopen(filename, "r", stdin);
    if (fp == NULL) {
        RAISE_ERROR(ERR_OPENING_FILE);
    }

    parseSTDIN();

    fclose(stdin);
}
