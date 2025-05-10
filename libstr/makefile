CC=gcc
CFLAG=-Wall -Wextra -Werror -Iinclude
SRC=$(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
TARGET = libstr

ifeq ($(OS),Windows_NT)
	SHELL := cmd
	RM = if exist build\\* del /Q build\\*
	RM_SHIP = if exist ship\\* del /Q ship\\*
	CLEAN_EXE = if exist $(TARGET)$(EXE) del $(TARGET)$(EXE)
	CLEAN_O = if exist $(TARGET).o del $(TARGET).o
	CLEAN_LIB = if exist $(TARGET).a del $(TARGET).a
	MKDIR = if not exist build mkdir build
	MKDIR_SHIP = if not exist ship mkdir ship
	COPY_A_SHIP = copy $(TARGET).a ship\\$(TARGET).a
	COPY_H_SHIP = copy include\\$(TARGET).h ship\\$(TARGET).h
	EXE = .exe
else
	RM = rm -f build/*.o
	RM_SHIP = rm -f ship/*.*
	CLEAN_EXE = rm -f $(TARGET)
	CLEAN_LIB = rm $(TARGET).a
	CLEAN_O = rm $(TARGET).o
	MKDIR = mkdir -p build
	MKDIR_SHIP = mkdir -p ship
	COPY_A_SHIP = cp $(TARGET).a ship//$(TARGET).a
	COPY_H_SHIP = cp include//$(TARGET).h shi//$(TARGET).h
	EXE =
endif

all:$(TARGET)_TEST$(EXE)
	@echo -------------------------
	@echo -- Test build finished --
	@echo -------------------------

$(TARGET)_TEST$(EXE): $(OBJ)
	$(CC) $(CFLAG) -o $@ $^

build/%.o: src/%.c
	$(MKDIR)
	$(CC) $(CFLAG) -c -o $@ $<

clean:
	$(RM)
	$(RM_SHIP)
	$(CLEAN_EXE)
	$(CLEAN_LIB)
	@echo --------------------
	@echo -- Clean finished --
	@echo --------------------

lib:
	$(CC) -c src/$(TARGET).c
	ar rcs $(TARGET).a $(TARGET).o
	$(CLEAN_O)
	$(MKDIR_SHIP)
	$(COPY_A_SHIP)
	$(COPY_H_SHIP)
	$(CLEAN_LIB)
	@echo ------------------------------------------------------------
	@echo -- The libstr.a and libstr.h files are in the ship folder --
	@echo ------------------------------------------------------------
	
.PHONY: all clean lib
