cmake_minimum_required(VERSION 3.16)
project(battleship C)

set(CMAKE_C_STANDARD 90)

add_executable(battleship main.c board.h tile.h ship.h types.h ship.c ship.c game.h game.h tile.c board.c board.c const.h main.h)
