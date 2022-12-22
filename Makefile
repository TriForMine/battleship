PROJDIR := $(realpath $(CURDIR))

SOURCEDIR = $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/build

CFLAGS  = -Wall -ansi -pedantic

SOURCE	= $(wildcard $(SOURCEDIR)/*.c)
HEADER	= $(wildcard $(SOURCEDIR)/*.h)
OBJS	= $(SOURCE:.c=.o)
TARGET	= ${BUILDDIR}/battleship
CC	 = gcc
LFLAGS	 =

# OS specific part
ifeq ($(OS),Windows_NT)
    RM = del /F /Q
    RMDIR = -RMDIR /S /Q
    MKDIR = -mkdir
    ERRIGNORE = 2>NUL || true
    SEP=\\
else
    RM = rm -rf
    RMDIR = rm -rf
    MKDIR = mkdir -p
    ERRIGNORE = 2>/dev/null
    SEP=/
endif

.PHONY: all clean directories release

all: directories $(TARGET)

directories:
	 @$(MKDIR) ${BUILDDIR} $(ERRIGNORE)

# Add -fsanitize=address,undefined flag if release target is not used
ifneq ($(MAKECMDGOALS),release)
    CFLAGS += --coverage
    CFLAGS += -fsanitize=address,undefined
    CFLAGS  += -funroll-loops -fno-omit-frame-pointer -fno-optimize-sibling-calls
    CFLAGS  += -Werror -O0 -g
    CFLAGS  += -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wstrict-prototypes -Wwrite-strings -Wunreachable-code
endif

release: CFLAGS += -static
release: all

$(TARGET): $(OBJS)
	@echo Linking $@
	$(CC) $(CFLAGS) -g $(OBJS) -o $(TARGET) $(LFLAGS)

clean:
	@${RM} -f $(OBJS) $(OUT) src/*.gcda src/*.gcno *~
	@${RMDIR} $(BUILDDIR)
	@echo Cleaning done!