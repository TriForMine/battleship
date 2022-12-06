PROJDIR := $(realpath $(CURDIR))

SOURCEDIR = $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/build

FLAGS  = -Wall -ansi -pedantic
FLAGS  += -funroll-loops -fsanitize=address,undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -fvisibility=hidden
FLAGS  += -Werror -Os -g
FLAGS  += -Wextra -Wno-unused-parameter -Wno-unused-variable -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wstrict-prototypes -Wwrite-strings -Wunreachable-code

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

.PHONY: all clean directories

all: directories $(TARGET)

directories:
	 @$(MKDIR) ${BUILDDIR} $(ERRIGNORE)

$(TARGET): $(OBJS)
	@echo Linking $@
	$(CC) ${FLAGS} -g $(OBJS) -o $(TARGET) $(LFLAGS)

clean:
	@${RM} -f $(OBJS) $(OUT) *~
	@${RMDIR} $(BUILDDIR)
	@echo Cleaning done!