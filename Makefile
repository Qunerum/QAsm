CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

SRC = src
OBJ = obj
BUILD = build

OBJS = $(OBJ)/main.o \
               $(OBJ)/memory.o \
               $(OBJ)/cmds.o \
               $(OBJ)/utility.o

TARGET = $(BUILD)/qasm_compiler

.PHONY: run clean

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

run: $(TARGET)
	@echo Compiled!

clean:
	@rm -rf $(OBJ) $(BUILD)
