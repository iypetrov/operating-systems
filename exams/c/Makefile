CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -pedantic-errors -O2 -c -o
RM = rm -f

SRC_FILE ?= file1/file1.c
BUILD_DIR = $(dir $(SRC_FILE))
OBJ_FILE = $(BUILD_DIR)main.o
TARGET = $(BUILD_DIR)main

build: $(TARGET)

$(TARGET): $(OBJ_FILE)
	$(CC) $(OBJ_FILE) -o $@

$(OBJ_FILE): $(SRC_FILE)
	$(CC) $(CFLAGS) $@ $<

clean:
	$(RM) -rf $(BUILD_DIR) $(TARGET)

.PHONY: build
