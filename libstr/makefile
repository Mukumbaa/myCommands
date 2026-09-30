CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -Iinclude
AR = ar
ARFLAGS = rcs

TARGET = libstr
TEST_BIN_NAME = $(TARGET)_TEST

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))

ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
    EXE = .exe
    TEST_BIN = $(TEST_BIN_NAME)$(EXE)
    LIB_OUT = ship\\$(TARGET)_win.a
    
    MKDIR = if not exist build mkdir build
    MKDIR_SHIP = if not exist ship mkdir ship
    
    RM_BUILD = if exist build\\* del /Q build\\*
    RM_SHIP  = if exist ship\\* del /Q ship\\*
    RM_TEST  = if exist $(TEST_BIN) del /Q $(TEST_BIN)
    
    COPY_LIB = copy /Y build\\$(TARGET).a $(LIB_OUT)
    COPY_HDR = copy /Y include\\$(TARGET).h ship\\$(TARGET).h
else
    EXE =
    TEST_BIN = $(TEST_BIN_NAME)
    LIB_OUT = ship/$(TARGET)_linux.a
    
    MKDIR = mkdir -p build
    MKDIR_SHIP = mkdir -p ship
    
    RM_BUILD = rm -rf build/*
    RM_SHIP  = rm -rf ship/*
    RM_TEST  = rm -f $(TEST_BIN)
    
    COPY_LIB = cp build/$(TARGET).a $(LIB_OUT)
    COPY_HDR = cp include/$(TARGET).h ship/$(TARGET).h
endif

# Target di default: compila l'eseguibile di test
all: $(TEST_BIN)
	@echo -------------------------
	@echo -- Test build finished --
	@echo -------------------------

# Linker per i test
$(TEST_BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compilazione file oggetto
build/%.o: src/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Esegue direttamente la suite di test
test: $(TEST_BIN)
	@echo -- Running tests...
ifeq ($(OS),Windows_NT)
	@$(TEST_BIN)
else
	@./$(TEST_BIN)
endif

# Crea la libreria statica (.a) e copia l'header dentro la cartella ship/
lib: build/$(TARGET).o
	$(MKDIR_SHIP)
	$(AR) $(ARFLAGS) build/$(TARGET).a build/$(TARGET).o
	$(COPY_LIB)
	$(COPY_HDR)
	@echo ------------------------------------------------------------
	@echo -- The static lib and header are ready in the ship/ folder -
	@echo ------------------------------------------------------------

# Pulizia completa di build/, ship/ e dell'eseguibile di test
clean:
	$(RM_BUILD)
	$(RM_SHIP)
	$(RM_TEST)
	@echo --------------------
	@echo -- Clean finished --
	@echo --------------------

.PHONY: all test lib clean
