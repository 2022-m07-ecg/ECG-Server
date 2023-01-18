TARGET = server
CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -g -Wall
MARIADB = -I/usr/include/mariadb -I/usr/include/mariadb/mysql -L/usr/lib/x86_64-linux-gnu/ -lmariadb
SRC_DIR = src
BUILD_DIR = build
CFILES = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CFILES))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(MARIADB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(MARIADB)

.PHONY: clean

clean:
	rm -f $(BUILD_DIR)/* $(TARGET) tmp/*

makedb:
	./make.sh

rundb:
	./db