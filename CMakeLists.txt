cmake_minimum_required(VERSION 3.16)
project(battleship C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

add_compile_options(-ansi)
add_executable(battleship src/main.c src/board.h src/tile.h src/ship.h src/types.h src/ship.c src/ship.c src/game.h src/game.h src/tile.c src/board.c src/board.c src/const.h src/main.h src/helpers.h src/helpers.c src/stdprof.c src/hmap.c src/hmap.h src/parser.c src/parser.h)
