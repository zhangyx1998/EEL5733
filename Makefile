# Created by Yuxuan Zhang,
# University of Florida
# SHELL := /bin/bash
USER  := Yuxuan_Zhang
# Compiler Parameters
CC      = gcc
CFLAGS  = -Wall
# Target list
TARGETS = email_filter calendar_filter location_updater
# Object list
OBJS    = util.o list.o
# Env related settings
BUILD_PATH = build
# Default target
all: $(BUILD_PATH) color_gray $(foreach t,$(TARGETS),$t.bin) color_restore
# Check for debug flag
debug: CFLAGS += -DDEBUG -g
debug: all
# Set up C suffixes & relationship between .cpp and .o files
$(BUILD_PATH):
	@mkdir -p $(BUILD_PATH)

%.o: src/%.c
	$(CC) $(CFLAGS) -c -I./inc -o $(BUILD_PATH)/$@ $<

%.bin: $(OBJS) %.o
	@echo $@
	cd $(BUILD_PATH); ${CC} -o $@ $(@:.bin=.o) $(OBJS);
	@chmod +x $(BUILD_PATH)/$@

# Change console output color
color_gray:
	@echo "\033[1;30m"
color_restore:
	@echo "\033[0m"

clean:
	rm -rf $(BUILD_PATH)

# Special target to zip everything for submission
BRANCH:=$(shell BR=$$(git branch --show-current); echo $$(tr '[:lower:]' '[:upper:]' <<< $${BR:0:1})$${BR:1})
FILE_LIST:=$(shell git ls-tree --full-tree --name-only -r HEAD)
ARCHIVE:=$(USER)_$(BRANCH)

zip:
	mkdir -p var
	@zip var/$(ARCHIVE).zip $(FILE_LIST)

.PHONY: clean $(BUILD_PATH) test

include ./test/Makefile