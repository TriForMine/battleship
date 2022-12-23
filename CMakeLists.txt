cmake_minimum_required(VERSION 3.16)
project(battleship C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

add_compile_options(-ansi)
add_executable(battleship src/main.c src/game/board.h src/utils/error.h src/game/tile.h src/game/ship.h src/utils/types.h src/game/ship.c src/game/ship.c src/game/game.h src/game/game.h src/utils/error.c src/game/tile.c src/game/board.c src/game/board.c src/utils/const.h src/main.h src/utils/helpers.h src/utils/helpers.c src/utils/stdprof.c src/utils/hmap.c src/utils/hmap.h src/core/parser.c src/core/parser.h src/game/game.c src/utils/random.c src/utils/random.h src/core/interactive.c src/core/interactive.h src/ai/random_ai.h src/ai/random_ai.c src/ai/common.c src/ai/common.h src/ai/common.h)