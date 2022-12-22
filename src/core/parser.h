#ifndef BATTLESHIP_PARSER_H
#define BATTLESHIP_PARSER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "../game/game.h"
#include "../game/board.h"
#include "../utils/error.h"
#include "../utils/stdprof.h"
#include "../game/ship.h"

void parseSTDIN(void);
void parseFile(char* filename);

#endif /*BATTLESHIP_PARSER_H*/
