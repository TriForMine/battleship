#ifndef BATTLESHIP_COLORS_H
#define BATTLESHIP_COLORS_H

#ifdef _WIN32
#include <windows.h>
#define SET_TEXT_COLOR(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color)

#define RESET_TEXT_COLOR()    SET_TEXT_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

#define SET_RED_TEXT()        SET_TEXT_COLOR(FOREGROUND_RED)
#define SET_GREEN_TEXT()      SET_TEXT_COLOR(FOREGROUND_GREEN)
#define SET_YELLOW_TEXT()     SET_TEXT_COLOR(FOREGROUND_RED | FOREGROUND_GREEN)
#define SET_BLUE_TEXT()       SET_TEXT_COLOR(FOREGROUND_BLUE)
#define SET_MAGENTA_TEXT()    SET_TEXT_COLOR(FOREGROUND_RED | FOREGROUND_BLUE)
#define SET_CYAN_TEXT()       SET_TEXT_COLOR(FOREGROUND_GREEN | FOREGROUND_BLUE)
#else
#include <unistd.h>
#define SET_TEXT_COLOR(color) printf("\x1b[%dm", color)

#define RESET_TEXT_COLOR()    SET_TEXT_COLOR(0)

#define SET_RED_TEXT()        SET_TEXT_COLOR(31)
#define SET_GREEN_TEXT()      SET_TEXT_COLOR(32)
#define SET_YELLOW_TEXT()     SET_TEXT_COLOR(33)
#define SET_BLUE_TEXT()       SET_TEXT_COLOR(34)
#define SET_MAGENTA_TEXT()    SET_TEXT_COLOR(35)
#define SET_CYAN_TEXT()       SET_TEXT_COLOR(36)
#endif

#define PRINT_RED_TEXT(text)                                                                                           \
    SET_RED_TEXT();                                                                                                    \
    printf(text);                                                                                                      \
    RESET_TEXT_COLOR()
#define PRINT_GREEN_TEXT(text)                                                                                         \
    SET_GREEN_TEXT();                                                                                                  \
    printf(text);                                                                                                      \
    RESET_TEXT_COLOR()
#define PRINT_YELLOW_TEXT(text)                                                                                        \
    SET_YELLOW_TEXT();                                                                                                 \
    printf(text);                                                                                                      \
    RESET_TEXT_COLOR()
#define PRINT_BLUE_TEXT(text)                                                                                          \
    SET_BLUE_TEXT();                                                                                                   \
    printf(text);                                                                                                      \
    RESET_TEXT_COLOR()
#define PRINT_MAGENTA_TEXT(text)                                                                                       \
    SET_MAGENTA_TEXT();                                                                                                \
    printf(text);                                                                                                      \
    RESET_TEXT_COLOR()
#define PRINT_CYAN_TEXT(text)                                                                                          \
    SET_CYAN_TEXT();                                                                                                   \
    printf(text);                                                                                                      \
    RESET_TEXT_COLOR()

#endif /* BATTLESHIP_COLORS_H */