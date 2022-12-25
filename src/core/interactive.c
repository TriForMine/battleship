#include "interactive.h"
#include "../utils/colors.h"

Game* startGame(int shipLengths[], int shipLengthsLengths) {
    Game* game;

    int i;
    char buffer[BUFFER_SIZE];

    AI_Mode ai_mode;
    unsigned int WIDTH;
    unsigned int HEIGHT;
    unsigned int seed = generate_random_seed();
    Coordinate coordinate;
    Orientation orientation;

    printf("Please enter the ");
    PRINT_BLUE_TEXT("width");
    printf(" of the board ");
    PRINT_YELLOW_TEXT("(10)");
    printf(": ");

    if (fgets(buffer, BUFFER_SIZE, stdin) != NULL && strcmp(buffer, "\n") != 0) {
        WIDTH = strtol(buffer, NULL, 10);
    } else {
        WIDTH = 10;
    }

    printf("Please enter the ");
    PRINT_BLUE_TEXT("height");
    printf(" of the board ");
    PRINT_YELLOW_TEXT("(10)");
    printf(": ");

    if (fgets(buffer, BUFFER_SIZE, stdin) != NULL && strcmp(buffer, "\n") != 0) {
        HEIGHT = strtol(buffer, NULL, 10);
    } else {
        HEIGHT = 10;
    }

    printf("Please enter the ");

    PRINT_BLUE_TEXT("ai mode");

    printf(" (");

    PRINT_YELLOW_TEXT("0");

    printf(" = disabled, ");

    PRINT_YELLOW_TEXT("1");

    printf(" = random, ");

    PRINT_YELLOW_TEXT("2");

    printf(" = hunt and target, ");
    PRINT_YELLOW_TEXT("3");

    printf(" = probability targeting) ");
    PRINT_YELLOW_TEXT("(3)");
    printf(": ");

    if (fgets(buffer, BUFFER_SIZE, stdin) != NULL && strcmp(buffer, "\n") != 0) {
        ai_mode = strtol(buffer, NULL, 10);
    } else {
        ai_mode = PROBABILITY_TARGETING;
    }

    if (ai_mode != DISABLED) {
        printf("\nGame will be played against the");
        SET_BLUE_TEXT();
        printf(" %s ", ai_mode == RANDOM ? "random" : ai_mode == HUNT_TARGET ? "hunt and target" : "probability targeting");
        RESET_TEXT_COLOR();
        printf("AI.\n");

        printf("The game seed is");
        SET_RED_TEXT();
        printf(" %u", seed);
        RESET_TEXT_COLOR();
        printf(".\n\n");
    }

    game = createGame(false, ai_mode, seed, WIDTH, HEIGHT);

    /* Ask if he wants to place manually or automatically */
    printf("Do you want to place your ships manually? (y/n): ");
    fgets(buffer, BUFFER_SIZE, stdin);
    if (buffer[0] == 'y') {
        /* Place manually */
        for (i = 0; i < shipLengthsLengths; i++) {
            printBoard(getCurrentPlayerBoard(game), true);
            printf("Please enter the orientation of the %d ship (", shipLengths[i]);

            PRINT_RED_TEXT("0");

            printf(" = horizontal, ");

            PRINT_YELLOW_TEXT("1");

            printf(" = vertical): ");

            fgets(buffer, BUFFER_SIZE, stdin);
            orientation = strtol(buffer, NULL, 10);
            printf("Please enter the coordinate of the %d ship (e.g. A1): ", shipLengths[i]);
            fgets(buffer, BUFFER_SIZE, stdin);
            parseCoordinate(buffer, &coordinate, shipLengths[i], orientation, WIDTH, HEIGHT);
            placeShip(getPlayerBoard(game, '1'), createShip(shipLengths[i], orientation), coordinate);
            clearConsole();
        }
    } else {
        /* Place automatically */
        placeRandomShips(getPlayerBoard(game, '1'), shipLengths, shipLengthsLengths);
    }

    placeRandomShips(getPlayerBoard(game, '2'), shipLengths, shipLengthsLengths);

    game->state = PLAYING;
    setGameTurnToPlayer1(game);

    return game;
}

void printCurrentGame(Game* game) {
    clearConsole();
    printGame(game);
    printf("\nPlease use one of the following commands:\n");
    printf("1. Attack a coordinate (e.g. fire A1)\n");
    printf("2. Move a ship (e.g. move A1 D)\n");
    printf("3. Quit (e.g. exit)\n\n");
}

void handleInteractiveGame(void) {
    int shipLengths[] = {5, 4, 3, 3, 2};
    int shipLengthsLengths = 5;

    char buffer[BUFFER_SIZE];
    Game* game = NULL;

    SET_BLUE_TEXT();
    printf("Battleship %s\n", PROGRAM_VERSION);
    RESET_TEXT_COLOR();

    printf("Written by: Quentin Nicolini and Samy Ben dhiab\n\n");
    game = startGame(shipLengths, shipLengthsLengths);

    printCurrentGame(game);
    while (game->state != ENDED && fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        if (false == parseLine(game, buffer)) {
            printCurrentGame(game);
            continue;
        }

        if (game->state == PLAYING && checkVictory(game)) {
            game->state = ENDED;
            clearConsole();
            printGame(game);
            printf("Victory of player: 1\n");
            break;
        }

        if (game->ai_mode != DISABLED) {
            performAiTurn(game);
        }

        if (game->state == PLAYING && checkVictory(game)) {
            game->state = ENDED;
            clearConsole();
            printGame(game);
            printf("Victory of player: 2\n");
            break;
        }
        printCurrentGame(game);
    }
    freeGame(game);
}

void performAiTurn(Game* game) {
    if (game->state == PLAYING) {
        switch (game->ai_mode) {
            case RANDOM:
                playRandomAI(game);
                break;
            case HUNT_TARGET:
                playHuntTargetAI(game);
                break;
            case PROBABILITY_TARGETING:
                playProbabilityTargetingAI(game);
                break;
            default:
                return;
        }
    }
}

bool parseLine(Game* game, char* line) {
    int count;
    Coordinate coordinate;
    char str_coordinate[4];
    char param;
    Ship* ship;
    Board* board;
    char command[11];
    count = sscanf(line, "%10s %3s %c", command, str_coordinate, &param);
    board = getPlayerBoard(game, '1');

    parseCoordinate(str_coordinate, &coordinate, 0, -1, game->board1->WIDTH, game->board1->HEIGHT);

    if (count == 3) {
        if (strcmp(command, "move") == 0) {
            ship = getShip(board, coordinate);
            if (isShipSunk(ship)) {
                printf("You can't move a sunk ship!\n");
                return false;
            }

            switch (param) {
                case 'H':
                    moveUp(game, coordinate);
                    break;
                case 'B':
                    moveDown(game, coordinate);
                    break;
                case 'G':
                    moveLeft(game, coordinate);
                    break;
                case 'D':
                    moveRight(game, coordinate);
                    break;
                default:
                    printf("Invalid parameter for move command!\n");
                    return false;
            }
        } else {
            printf("Invalid command!\n");
            return false;
        }
    } else if (count == 2) {
        if (strcmp(command, "fire") == 0) {
            if (getTileState(getOtherPlayerBoard(game), coordinate) != MINE) {
                fire(game, coordinate);
            } else {
                printf("You already fired at this coordinate!\n");
                return false;
            }
        } else {
            printf("Invalid command!\n");
            return false;
        }
    } else if (count == 1) {
        if (strcmp(command, "reset") == 0) {
            resetGame(game, generate_random_seed());
        } else if (strcmp(command, "exit") == 0) {
            game->state = ENDED;
        } else {
            printf("Unknown command: %s\n", command);
            return false;
        }
    } else {
        printf("Unknown command: %s\n", command);
        return false;
    }

    return true;
}

void parseCoordinate(const char* str, Coordinate* coordinate, unsigned int size, const int orientation,
                     unsigned int WIDTH, unsigned int HEIGHT) {
    char letter = str[0];
    if (letter >= 'a' && letter <= 'z') {
        letter -= 'a' - 'A';
    } else if (letter < 'A' || letter > 'Z') {
        letter = 'A';
    }
    coordinate->x = strtol(str + 1, NULL, 10);
    coordinate->y = letter - 'A';

    if (orientation == 0) {
        if (coordinate->x + size > WIDTH) {
            coordinate->x = WIDTH - size;
        }
    } else if (orientation == 1) {
        if (coordinate->y + size > HEIGHT) {
            coordinate->y = HEIGHT - size;
        }
    } else {
        if (coordinate->x > WIDTH) {
            coordinate->x = WIDTH - size;
        }
        if (coordinate->y > HEIGHT) {
            coordinate->y = HEIGHT;
        }
    }
}
