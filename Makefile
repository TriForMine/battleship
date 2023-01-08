# Configurations

PROGRAM_NAME = battleship
PROGRAM_VERSION = v1.0.0

# Do not to what comes after this line

PROJDIR := $(realpath $(CURDIR))

SOURCEDIR = $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/build

CFLAGS  = -Wall -ansi -pedantic
CFLAGS += -DPROGRAM_VERSION='"$(PROGRAM_VERSION)"'

# Include all source files in subdirectories of SOURCEDIR
SOURCEDIRS = $(shell find $(SOURCEDIR) -type d)
SOURCES = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.c))
HEADERS =  $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.h))

# Generate a list of object files from the source files, with the directory name as a prefix
OBJS = $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

TARGET	= ${BUILDDIR}/${PROGRAM_NAME}
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
	$(foreach obj,$(OBJS),$(MKDIR) $(dir $(obj)))

# Add -fsanitize=address,undefined flag if release target is not used
ifneq ($(MAKECMDGOALS),release)
    CFLAGS += -fsanitize=address,undefined
    CFLAGS  += -funroll-loops -fno-omit-frame-pointer -fno-optimize-sibling-calls
    CFLAGS  += -Werror -O0 -g
    CFLAGS  += -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wstrict-prototypes -Wwrite-strings -Wunreachable-code
endif

debug: CFLAGS += -DDEBUG_MODE
debug: all

release: CFLAGS += -static
release: all

# Build server from server/cargo.tml with cargo build --release
server: directories
	cd $(SOURCEDIR)/../server && cargo build --release
	cp $(SOURCEDIR)/../server/target/release/server $(BUILDDIR)/server

$(TARGET): $(OBJS)
	@echo Linking $@
	$(CC) $(CFLAGS) -g $(OBJS) -o $(TARGET) $(LFLAGS)

$(OBJS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@${RM} -f $(OBJS) $(TARGET) *~
	@${RMDIR} $(BUILDDIR)
	@echo Cleaning done!