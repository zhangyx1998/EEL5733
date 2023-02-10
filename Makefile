# Created by Yuxuan Zhang,
# University of Florida
# SHELL := /bin/bash
USER  := Yuxuan_Zhang
# Env related settings
BUILD = build
# Compiler Parameters
CC      = gcc
CFLAGS  = -Wall
# Object list
SRCS     = $(wildcard src/*.c)
INCS     = $(wildcard inc/*.h)
OBJS     = $(patsubst src/%.c,$(BUILD)/%.o,$(SRCS))
LIB_INCS = $(wildcard lib/*.h)
LIB_SRCS = $(wildcard lib/*.c)
LIB_OBJS = $(patsubst %.c,$(BUILD)/%.o,$(LIB_SRCS))
TARGETS  = $(patsubst src/%.c,%.bin,$(SRCS))
# Default target
all: color_gray $(TARGETS) color_restore
# Check for debug flag
debug: CFLAGS += -DDEBUG -g
debug: all
# Set up C suffixes & relationship between .cpp and .o files
$(BUILD)/src/%.o: src/%.c $(INCS) $(LIB_INCS)
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c -I./inc -I./lib -o $@ $<

$(BUILD)/lib/%.o: lib/%.c $(LIB_INCS)
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c -I./lib -o $@ $<

%.bin: $(BUILD)/src/%.o $(LIB_OBJS)
	@$(eval EXEC=$(BUILD)/$(@:.bin=))
	${CC} -o $(EXEC) $(BUILD)/src/$(@:.bin=.o) $(LIB_OBJS);
	@chmod +x $(EXEC)

# Change console output color
color_gray:
	@echo "\033[1;30m"
color_restore:
	@echo "\033[0m"

clean:
	rm -rf $(BUILD)

# Special target to zip everything for submission
BRANCH:=$(shell BR=$$(git branch --show-current); echo $$(tr '[:lower:]' '[:upper:]' <<< $${BR:0:1})$${BR:1})
FILE_LIST:=$(shell git ls-tree --full-tree --name-only -r HEAD)
ARCHIVE:=$(USER)_$(BRANCH)

zip:
	mkdir -p var
	@zip var/$(ARCHIVE).zip $(FILE_LIST)

.PHONY: clean $(BUILD) test

include ./test/Makefile
