CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build

COMPILER_OBJ = $(BIN_DIR)/main.o \
               $(BIN_DIR)/memory.o \
               $(BIN_DIR)/cmds.o \
               $(BIN_DIR)/utility.o

COMPILER_TARGET = $(BUILD_DIR)/qasm_compiler

ASM = nasm
ASM_SRC = bin/main.asm
ASM_OBJ = $(BIN_DIR)/main_asm.o
FINAL_TARGET = $(BUILD_DIR)/qcc

.PHONY: all compile_c run_compiler build_asm run clean

all: $(FINAL_TARGET)

# Krok A: Budowanie aplikacji w C (Kompilatora QAsm)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(COMPILER_TARGET): $(COMPILER_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(COMPILER_OBJ) -o $(COMPILER_TARGET)

# Krok B: Uruchomienie kompilatora w C, żeby wypluł plik main.asm
# (Założyłem, że qasm_compiler domyślnie generuje plik main.asm w głównym folderze)
$(ASM_SRC): $(COMPILER_TARGET)
	@echo "--- [QCC] Uruchamianie kompilatora w C... ---"
	./$(COMPILER_TARGET)

# Krok C: Kompilacja pliku main.asm do formatu ELF64 przy użyciu NASM
$(ASM_OBJ): $(ASM_SRC)
	@mkdir -p $(BIN_DIR)
	@echo "--- [QCC] Kompilacja assemblera (NASM)... ---"
	$(ASM) -f elf64 $(ASM_SRC) -o $(ASM_OBJ)

# Krok D: Linkowanie binarnego pliku ASM do finalnego programu na Linuxa
$(FINAL_TARGET): $(ASM_OBJ)
	@mkdir -p $(BUILD_DIR)
	@echo "--- [QCC] Linkowanie finalnej binarki (LD)... ---"
	ld $(ASM_OBJ) -o $(FINAL_TARGET)

# --- DODATKOWE POMOCNICZE REGUŁY ---

# Szybkie uruchomienie skompilowanego programu napisanego w Twoim języku
run: all
	@clear
	@echo "--- Uruchamianie wygenerowanego programu ---"
	./$(FINAL_TARGET)

# Czyszczenie całego środowiska
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR) $(ASM_SRC)
