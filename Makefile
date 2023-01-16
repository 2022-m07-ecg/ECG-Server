TARGET = server
CC = gcc
SRC_DIR = ./src
BUILD_DIR = ./build
OBJS = main.o

all: $(TARGET)

$(TARGET): $(BUILD_DIR)/$(OBJS)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

.PHONE: clean

clean:
	rm -f $(BUILD_DIR)/* $(TARGET)