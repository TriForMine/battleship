#include "parser.h"

#define BUFFER_SIZE     256
#define MAX_LENGTH_NAME 100

void parseSTDIN(void) {
    Board* board = NULL;

    char buffer[BUFFER_SIZE];
    unsigned int width, height;
    bool placing_mode = true;
    char action;
    char target[BUFFER_SIZE];
    int player;

    /* Read the input line by line */
    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        if (buffer[0] == '#') {
            /* This is a comment, ignore it */
            continue;
        } else if (sscanf(buffer, "Projet %ux%u", &height, &width) == 2) {
            /* This line defines the size of the board */
            printf("Creating board of size %u x %u\n", width, height);

            /* Create the board */
            board = createBoard(width, height);
        } else if (strstr(buffer, "Jouer") != NULL) {
            /* This line signals the start of the game */
            placing_mode = false;
        } else if (sscanf(buffer, "J%d %c", &player, &action) == 2) {
            /* This line represents a player action */

            if (action == 'T') {
                unsigned int pos_x, pos_y;

                /* This action has a target */
                sscanf(buffer, "J%d %c %u:%u", &player, &action, &pos_x, &pos_y);
                fireAt(board, createCoordinate(pos_x, pos_y));

            } else if (action == 'G' || action == 'D' || action == 'B' || action == 'H') {
                Ship* ship = NULL;
                char direction;
                char movement_direction;
                char* ship_name = malloc_prof(sizeof(char) * MAX_LENGTH_NAME);
                unsigned int pos_x, pos_y;

                if (sscanf(buffer, "J%d %c %u:%u", &player, &direction, &pos_x, &pos_y) == 4) {
                    ship = getShip(board, createCoordinate(pos_x, pos_y));
                } else if (sscanf(buffer, "J%d %c \"%[^\"]\"", &player, &direction, ship_name) == 3) {
                    ship = getShipWithName(board, ship_name);
                } else {
                    printf("Invalid move command: %s", buffer);
                    continue;
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
                    default:
                        movement_direction = UP;
                        break;
                }

                moveShip(board, ship, movement_direction);

                free_prof(ship_name);
            } else if (action == 'P') {
                int i;
                unsigned int start_x, start_y, end_x, end_y, f;
                char* ship_name = malloc_prof(sizeof(char) * MAX_LENGTH_NAME);
                Ship* ship;

                if (false == placing_mode) {
                    printf("Error: Placing ships is not allowed at this time\n");
                    continue;
                }
                if (sscanf(buffer, "J%d P \"%[^\"]\" %u:%u-%u", &player, ship_name, &start_x, &start_y, &end_y) == 5) {
                    /* This is a vertical ship */
                    end_x = start_x;

                    ship = createShipWithName(board, end_y - start_y + 1, VERTICAL, ship_name);
                } else if (sscanf(buffer, "J%d P \"%[^\"]\" %u-%u:%u", &player, ship_name, &start_x, &end_x, &start_y)
                           == 5) {
                    /* This is a horizontal ship */
                    end_y = start_y;

                    ship = createShipWithName(board, end_x - start_x + 1, HORIZONTAL, ship_name);
                } else if (sscanf(buffer, "J%d P \"%[^\"]\" %u:%u", &player, ship_name, &start_x, &start_y) == 4) {
                    /* This is a horizontal ship */
                    end_x = start_x;
                    end_y = start_y;

                    ship = createShipWithName(board, 1, HORIZONTAL, ship_name);
                } else if (sscanf(buffer, "J%d P %u:%u-%u", &player, &start_x, &start_y, &end_y) == 4) {
                    /* This is a vertical ship */
                    end_x = start_x;

                    ship = createShip(end_y - start_y + 1, VERTICAL);
                    free_prof(ship_name);
                } else if (sscanf(buffer, "J%d P %u-%u:%u", &player, &start_x, &end_x, &start_y) == 4) {
                    /* This is a horizontal ship */
                    end_y = start_y;

                    ship = createShip(end_x - start_x + 1, HORIZONTAL);
                    free_prof(ship_name);
                } else if (sscanf(buffer, "J%d P %u:%u", &player, &start_x, &start_y) == 3) {
                    /* This is a horizontal ship */
                    end_x = start_x;
                    end_y = start_y;

                    ship = createShip(1, HORIZONTAL);
                    free_prof(ship_name);
                } else {
                    printf("Invalid ship placement command: %s", buffer);
                    continue;
                }

                placeShip(board, ship, createCoordinate(start_x, start_y));
            } else {
                strcpy(target, "");
            }
        } else if (strstr(buffer, "Afficher") != NULL) {
            printf("Printing board\n");
            printBoard(board);
        } else {
            printf("Unknown command: %s", buffer);
        }
    }

    /* Free the board */
    if (board != NULL) {
        freeBoard(board);
    }
}

void parseFile(char* filename) {
    FILE* fp = freopen(filename, "r", stdin);
    if (fp == NULL) {
        perror("Error opening file");
    }

    parseSTDIN();

    fclose(stdin);
}
