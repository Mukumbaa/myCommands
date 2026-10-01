CC=gcc
CFLAG=-Wall -Wextra -Werror
SRC=$(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))

ifeq ($(OS),Windows_NT)
	LIBFLAG=-Llib -lstr_win
	TARGET = rgx_win
	SHELL := cmd
	RM = if exist build\\*.o del /Q build\\*.o
	CLEAN_EXE = if exist $(TARGET)$(EXE) del $(TARGET)$(EXE)
	MKDIR = if not exist build mkdir build
	EXE = .exe
else
	LIBFLAG=-Llib -lstr_linux
	TARGET = rgx_linux
	RM = rm -f build/*.o
	CLEAN_EXE = rm -f $(TARGET)$(EXE)
	MKDIR = mkdir -p build
	EXE = .out
endif

all: $(TARGET)$(EXE)

$(TARGET)$(EXE): $(OBJ)
	$(CC) $(CFLAG) -o $@ $^ $(LIBFLAG)

build/%.o: src/%.c
	$(MKDIR)
	$(CC) $(CFLAG) -c -o $@ $<

clean:
	$(RM)
	$(CLEAN_EXE)

.PHONY: all clean
