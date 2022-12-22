PROJDIR := $(realpath $(CURDIR))

SOURCEDIR = $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/build

CFLAGS  = -Wall -ansi -pedantic

# Include all source files in subdirectories of SOURCEDIR
SOURCEDIRS = $(shell find $(SOURCEDIR) -type d)
SOURCES = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.c))
HEADERS =  $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.h))

# Generate a list of object files from the source files, with the directory name as a prefix
OBJS = $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

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
	$(foreach obj,$(OBJS),$(MKDIR) $(dir $(obj)))

# Add -fsanitize=address,undefined flag if release target is not used
ifneq ($(MAKECMDGOALS),release)
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

$(OBJS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@${RM} -f $(OBJS) $(TARGET) *~
	@${RMDIR} $(BUILDDIR)
	@echo Cleaning done!