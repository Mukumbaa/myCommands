CC=gcc
CFLAG=-Wall -Wextra -Werror
SRC=$(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
TARGET = ftree


ifeq ($(OS),Windows_NT)
	SYS = _win
	SHELL := cmd
	RM = if exist build\\*.o del /Q build\\*.o
	CLEAN_EXE = if exist $(TARGET)$(SYS)$(EXE) del $(TARGET)$(SYS)$(EXE)
	MKDIR = if not exist build mkdir build
	EXE = .exe
else
	SYS = _linux
	RM = rm -f build/*.o
	CLEAN_EXE = rm -f $(TARGET)$(SYS)$(EXE)
	MKDIR = mkdir -p build
	EXE = .out
endif

all: $(TARGET)$(SYS)$(EXE)

$(TARGET)$(SYS)$(EXE): $(OBJ)
	$(CC) $(CFLAG) -o $@ $^

build/%.o: src/%.c
	$(MKDIR)
	$(CC) $(CFLAG) -c -o $@ $<

clean:
	$(RM)
	$(CLEAN_EXE)

.PHONY: all clean
