TARGET = server
CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -g -Wall
SRC_DIR = src
BUILD_DIR = build
CFILES = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CFILES))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(BUILD_DIR)/* $(TARGET) tmp/*

makedb:
        ./make.sh

rundb:
        ./db