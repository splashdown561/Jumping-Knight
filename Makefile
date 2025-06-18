# Cross-platform Makefile for Raylib Game

# Detect OS
ifeq ($(OS),Windows_NT)
    # Windows settings
    EXEEXT = .exe
    CC     = clang
    CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude/
    # Link against Windows libraries for Raylib
    LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32
    RM     = del /Q
    MKDIR  = if not exist "$(dir $@)" mkdir "$(dir $@)"
else
    # Unix-like settings
    EXEEXT = 
    CC     = clang
    CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude/
    LDFLAGS = -lraylib -lm -ldl -lpthread -lGL
    RM     = rm -rf
    MKDIR  = mkdir -p
endif

SRC_DIR = src
OBJ_DIR = obj
SRCS    = $(shell find $(SRC_DIR) -name '*.c')
OBJS    = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET  = game$(EXEEXT)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	$(RM) $(OBJ_DIR) $(TARGET)
