cmake_minimum_required(VERSION 3.16)
project(battleship C)

# Enable CMP0077 policy to allow passing variables from the command line to CMake
cmake_policy(SET CMP0077 NEW)

set(CMAKE_C_STANDARD 90)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
add_compile_options(-ansi)

# Set PROGRAM_VERSION variable to default value if not set from command line
if (NOT DEFINED PROGRAM_VERSION)
    set(PROGRAM_VERSION "DEV")
endif ()
# Add PROGRAM_VERSION preprocessor definition
add_compile_definitions(PROGRAM_VERSION= "${PROGRAM_VERSION}")

if (DEBUG)
    add_compile_definitions(DEBUG_MODE=1)
endif ()

add_executable(battleship src/main.c src/game/board.h src/utils/error.h src/game/tile.h src/game/ship.h src/utils/types.h src/game/ship.c src/game/ship.c src/game/game.h src/game/game.h src/utils/error.c src/game/tile.c src/game/board.c src/game/board.c src/utils/const.h src/main.h src/utils/helpers.h src/utils/helpers.c src/utils/stdprof.c src/utils/hmap.c src/utils/hmap.h src/core/parser.c src/core/parser.h src/game/game.c src/utils/random.c src/utils/random.h src/core/interactive.c src/core/interactive.h src/ai/random_ai.h src/ai/random_ai.c src/ai/common.c src/ai/common.h src/ai/common.h src/ai/hunt_target.c src/ai/hunt_target.h src/ai/hunt_target.h src/ai/hunt_target.h src/ai/hunt_target.h src/utils/colors.h src/ai/probability_targeting.c src/ai/probability_targeting.h src/core/benchmark.c src/core/benchmark.h)