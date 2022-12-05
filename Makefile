CC = gcc

CFLAGS	= -Wall -std=c90 -ansi -pedantic
CFLAGS	+= -funroll-loops -fsanitize=address,undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -fvisibility=hidden
CFLAGS  += -Werror -Os -g
CFLAGS  += -Wextra -Wno-unused-parameter -Wno-unused-variable -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wstrict-prototypes -Wwrite-strings -Waggregate-return -Wunreachable-code

all: main

%: %.c
	$(CC) $(CFLAGS) $< -o $@