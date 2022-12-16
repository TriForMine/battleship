#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include "board.h"
#include "ship.h"
#include "stdprof.h"

#define BUFFER_SIZE 256
#define MAX_LENGTH_NAME 100

int main(int argc, char *argv[]) {
    Board *board = NULL;

    char buffer[BUFFER_SIZE];
    char *token;
    int width, height;
    bool placing_mode = true;

    // Check if a file was specified as an argument
    if (argc > 1) {
        // Open the file for reading
        FILE *fp = freopen(argv[1], "r", stdin);
        if (fp == NULL) {
            perror("Error opening file");
            return -1;
        }
    }

    // Read the input line by line
    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        // Extract the first token from the line
        token = strtok(buffer, " ");

        if (token[0] == '#') {
            // This is a comment, ignore it
            continue;
        } if (strcmp(token, "Projet") == 0) {
            // This line defines the size of the board
            // Extract the width and height from the line
            token = strtok(NULL, "x");
            width = strtol(token, NULL, 10);
            token = strtok(NULL, " ");
            height = strtol(token, NULL, 10);

            printf("Creating board of size %d x %d\n", width, height);

            // Create the board
            board = createBoard(width, height);
        } else if (strcmp(token, "Jouer") == 0 || strcmp(token, "Jouer\n") == 0 || strcmp(token, "Jouer\r\n") == 0) {
            // This line signals the start of the game
            placing_mode = false;
        } else if (strcmp(token, "J1") == 0 || strcmp(token, "J2") == 0) {
            // This line represents a player action
            // Extract the action type and target from the line
            int player;
            char action[BUFFER_SIZE];
            char target[BUFFER_SIZE];

            if (strcmp(token, "J1") == 0) {
                player = 1;
            } else {
                player = 2;
            }

            token = strtok(NULL, " ");
            strcpy(action, token);

            if (strcmp(action, "T") == 0) {
                int pos_x, pos_y;

                // This action has a target
                token = strtok(NULL, " ");
                sscanf(token,"%d:%d", &pos_x, &pos_y);

                fireAt(board, createCoordinate(pos_x, pos_y));
            } else if (strcmp(action, "P") == 0) {
                int start_x, start_y, end_x, end_y, ship_orientation, ship_type;
                char* ship_name = malloc_prof(sizeof(char) * MAX_LENGTH_NAME);
                Ship* ship;

                if (false == placing_mode) {
                    printf("Error: Placing ships is not allowed at this time\n");
                    continue;
                }

                token = strtok(NULL, "\0");

                int f = sscanf(token, "\"%[^\"]\"", ship_name);

                if (f != 0) {
                    token += strlen(ship_name) + sizeof(char) * 3;
                } else {
                    if (ship_name != NULL) {
                        free_prof(ship_name);
                    }
                    ship_name = NULL;
                }

                int i = sscanf(token, "%d:%d-%d", &start_x, &start_y, &end_y);

                if (i != 3) {
                    sscanf(token, "%d-%d:%d", &start_x, &end_x, &start_y);
                    end_y = start_y;
                } else {
                    end_x = start_x;
                }

                ship_orientation = (start_x == end_x) ? 1 : 0;
                ship_type = (ship_orientation == 1) ? end_y - start_y + 1 : end_x - start_x + 1;

                if (f != 0) {
                    ship = createShipWithName(ship_type, ship_orientation, ship_name);
                } else {
                    ship = createShip(ship_type, ship_orientation);
                }

                placeShip(board, ship, createCoordinate(start_x, start_y));
            } else {
                strcpy(target, "");
            }
        } else if (strcmp(token, "Afficher") == 0 || strcmp(token, "Afficher\n") == 0) {
            printf("Printing board\n");
            printBoard(board);
        } else {
            printf("Unknown command: %s", token);
        }
    }

    // Close the file if it was opened
    if (argc > 1) {
        fclose(stdin);
    }

    // Free the board
    if (board != NULL) {
        freeBoard(board);
    }

    return 0;
}