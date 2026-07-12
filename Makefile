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

ASM = nasm
ASM_SRC = $(OBJ)/main.asm
ASM_OBJ = $(OBJ)/main_asm.o
FINAL_TARGET = $(BUILD)/QAsm

.PHONY: all run clean

all: $(FINAL_TARGET)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD)
	@$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(ASM_SRC): $(TARGET)
	@echo "--- [QCC] Uruchamianie kompilatora w C... ---"
	@./$(TARGET)

$(ASM_OBJ): $(ASM_SRC)
	@mkdir -p $(OBJ)
	@echo "--- [QCC] Kompilacja assemblera (NASM)... ---"
	@$(ASM) -f elf64 $(ASM_SRC) -o $(ASM_OBJ)

$(FINAL_TARGET): $(ASM_OBJ)
	@mkdir -p $(BUILD)
	@echo "--- [QCC] Linkowanie finalnej binarki (LD)... ---"
	@ld $(ASM_OBJ) -o $(FINAL_TARGET)

run: all
# 	@clear
	@./$(FINAL_TARGET)

clean:
	@rm -rf $(OBJ) $(BUILD) $(ASM_SRC)
