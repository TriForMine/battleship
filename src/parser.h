#ifndef BATTLESHIP_PARSER_H
#define BATTLESHIP_PARSER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "game.h"
#include "board.h"
#include "error.h"
#include "stdprof.h"
#include "ship.h"

void parseSTDIN(void);
void parseFile(char* filename);

#endif /*BATTLESHIP_PARSER_H*/
