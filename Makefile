CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11 -Iinclude
LDFLAGS =

TARGET  = metrics_daemon
ARCH   ?= native

SRC_DIR = src
OBJ_DIR = build/$(ARCH)
BIN_DIR = bin/$(ARCH)

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf build bin

build_native_image:
	docker build -t metrics_daemon .

build_linux_image:
	docker build --platform=linux/amd64 -t metrics_daemon .