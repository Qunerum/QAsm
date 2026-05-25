CC_LINUX = gcc
# CC_WIN = i686-w64-mingw32-gcc # 32 bit
CC_WIN = x86_64-w64-mingw32-gcc # 64 bit

CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS_WIN = -static -static-libgcc -static-libstdc++

SRC_DIR = src
BIN = bin
BUILD_DIR = build

ifeq ($(OS),Windows_NT)
    PLATFORM = win
    EXT = .exe
    TARGET_CC = $(CC_WIN)
    MKDIR = mkdir -p
else
    PLATFORM = linux
    EXT =
    TARGET_CC = $(CC_LINUX)
    MKDIR = mkdir -p
endif

PLATFORM_DIR = $(BUILD_DIR)/$(PLATFORM)
TARGET = $(PLATFORM_DIR)/qcode$(EXT)

OBJ = $(BIN)/$(PLATFORM)/main.o \
      $(BIN)/$(PLATFORM)/memory.o \
      $(BIN)/$(PLATFORM)/mainCmds.o \
      $(BIN)/$(PLATFORM)/utility.o

all: linux win

linux:
	@$(MAKE) PLATFORM=linux EXT= CC_LINUX=gcc build_platform

win:
	@$(MAKE) PLATFORM=win EXT=.exe TARGET_CC=$(CC_WIN) LDFLAGS="$(LDFLAGS_WIN)" build_platform

build_platform: $(TARGET)

$(BIN)/$(PLATFORM)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(BIN)/$(PLATFORM)
	$(TARGET_CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	@$(MKDIR) $(PLATFORM_DIR)
	$(TARGET_CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

run: all
	@clear
	@echo "--- Running on $(PLATFORM) ---"
	./$(TARGET)

clean:
	rm -rf $(BIN) $(BUILD_DIR)

.PHONY: all linux win run clean build_platform
