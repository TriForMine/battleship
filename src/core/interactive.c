#include "interactive.h"

Game* startGame(int shipLengths[], int shipLengthsLengths) {
    Game* game;

    int i;
    char buffer[BUFFER_SIZE];

    AI_Mode ai_mode = DISABLED;
    unsigned int WIDTH = 10;
    unsigned int HEIGHT = 10;
    Coordinate coordinate;
    Orientation orientation;

    printf("Please enter the width of the board: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    WIDTH = strtol(buffer, NULL, 10);
    printf("Please enter the height of the board: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    HEIGHT = strtol(buffer, NULL, 10);
    printf("Please enter the AI mode (0 = disabled, 1 = random, 2 = smart, 3 = MCTF): ");
    fgets(buffer, BUFFER_SIZE, stdin);
    ai_mode = strtol(buffer, NULL, 10);
    if (ai_mode > 3) {
        ai_mode = DISABLED;
    }

    game = createGame(false, ai_mode, WIDTH, HEIGHT);

    /* Ask if he wants to place manually or automatically */
    printf("Do you want to place your ships manually? (y/n): ");
    fgets(buffer, BUFFER_SIZE, stdin);
    if (buffer[0] == 'y') {
        /* Place manually */
        for (i = 0; i < shipLengthsLengths; i++) {
            printBoard(getCurrentPlayerBoard(game), true);
            printf("Please enter the orientation of the %d ship (0 = horizontal, 1 = vertical): ", shipLengths[i]);
            fgets(buffer, BUFFER_SIZE, stdin);
            orientation = strtol(buffer, NULL, 10);
            printf("Please enter the coordinate of the %d ship (e.g. A1): ", shipLengths[i]);
            fgets(buffer, BUFFER_SIZE, stdin);
            parseCoordinate(buffer, &coordinate);
            placeShip(getPlayerBoard(game, '1'), createShip(shipLengths[i], orientation, '1'), coordinate);
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
    printf("3. Quit (e.g. quit)\n\n");
}

void handleInteractiveGame(void) {
    int shipLengths[] = {5, 4, 3, 3, 2};
    int shipLengthsLengths = 5;

    char buffer[BUFFER_SIZE];
    Game* game = NULL;

    printf("Welcome to Battleship!\n");
    game = startGame(shipLengths, shipLengthsLengths);
    game->remaining_ships[0] = 5;
    game->remaining_ships[1] = 5;

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
            default:
                return;
        }
    }
}

bool parseLine(Game* game, char* line) {
    int count;
    Coordinate coordinate;
    char str_coordinate[3];
    char param;
    Ship* ship;
    Board* board;
    char command[BUFFER_SIZE];
    count = sscanf(line, "%s %2s %c", command, str_coordinate, &param);
    board = getPlayerBoard(game, '1');

    parseCoordinate(str_coordinate, &coordinate);

    if (count == 3) {
        if (strcmp(command, "move") == 0) {
            ship = getShip(board, coordinate);
            if (isShipSunk(ship)) {
                printf("You can't move a sunk ship!\n");
                return false;
            }

            switch (param) {
                case 'H':
                    moveUp(board, coordinate);
                    break;
                case 'B':
                    moveDown(board, coordinate);
                    break;
                case 'G':
                    moveLeft(board, coordinate);
                    break;
                case 'D':
                    moveRight(board, coordinate);
                    break;
                default:
                    printf("Invalid parameter for move command!\n");
                    return false;
            }

            setGameTurnToOpponent(game);
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
            resetGame(game);
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

void parseCoordinate(const char* str, Coordinate* coordinate) {
    coordinate->x = strtol(str + 1, NULL, 10);
    coordinate->y = str[0] - 'A';
}