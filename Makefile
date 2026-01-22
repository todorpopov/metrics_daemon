VERSION = $(shell cat VERSION.txt)
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11 -DVERSION=\"$(VERSION)\" -Iinclude -Iinclude/metrics -Iinclude/server
LDFLAGS =

TARGET  = metrics_daemon
ARCH   ?= native

SRC_DIR = src
OBJ_DIR = build/$(ARCH)
BIN_DIR = bin/$(ARCH)

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf build bin

build_arm_image:
	docker build --build-arg VERSION=$(VERSION) --platform=linux/arm64 -t metrics_daemon:$(VERSION) -t metrics_daemon:latest .

build_x86_image:
	docker buildx build --platform linux/amd64 -t infra:30500/metrics_daemon:$(VERSION) -t infra:30500/metrics_daemon:latest --push .
