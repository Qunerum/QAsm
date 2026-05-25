ASM = nasm
CC = gcc

MAIN = main.asm

BIN = bin
BUILD = build

TARGET = $(BUILD)/qcc

.PHONY: all
all: $(TARGET)

$(BIN)/main.o: $(MAIN)
	@mkdir -p $(BIN)
	$(ASM) -f elf64 $(MAIN) -o $(BIN)/main.o

$(TARGET): $(BIN)/main.o
	@mkdir -p $(BUILD)
	ld $(BIN)/main.o -o $(TARGET)

run: all
	@clear
	@./$(TARGET)

clean:
	rm -rf $(BIN) $(BUILD)

.PHONY: run clean
