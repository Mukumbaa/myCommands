CC=gcc
CFLAG=-Wall -Wextra -Werror -Iinclude
SRC=$(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
TARGET = strlib

ifeq ($(OS),Windows_NT)
	SHELL := cmd
	RM = if exist build\\*.o del /Q build\\*.o
	CLEAN_EXE = if exist $(TARGET)$(EXE) del $(TARGET)$(EXE)
	MKDIR = if not exist build mkdir build
	EXE = .exe
else
	RM = rm -f build/*.o
	CLEAN_EXE = rm -f $(TARGET)
	MKDIR = mkdir -p build
	EXE =
endif

all: $(TARGET)$(EXE)

$(TARGET)$(EXE): $(OBJ)
	$(CC) $(CFLAG) -o $@ $^

build/%.o: src/%.c
	$(MKDIR)
	$(CC) $(CFLAG) -c -o $@ $<

clean:
	$(RM)
	$(CLEAN_EXE)

.PHONY: all clean
