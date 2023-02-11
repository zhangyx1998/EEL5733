# Created by Yuxuan Zhang,
# University of Florida
# SHELL := /bin/bash
AUTHOR	?= Yuxuan_Zhang
# Env related settings
BUILD	?= build
# Compiler Parameters
CC		?= gcc
CFLAGS	?= -Wall
# Object list
SRCS     = $(wildcard src/*.c)
INCS     = $(wildcard inc/*.h)
LIB_INCS = $(wildcard lib/*.h)
LIB_SRCS = $(wildcard lib/*.c)
# ESCAPE Codes
BOLD = $(shell tput bold)
SMUL = $(shell tput smul)
RMUL = $(shell tput rmul)
SMSO = $(shell tput smso)
RMSO = $(shell tput rmso)
RESET = $(shell tput sgr0)
# Default target
all: $(patsubst src/%.c,$(BUILD)/%,$(SRCS))
# Check for debug flag
debug:
	$(eval CFLAGS=$(CFLAGS) -DDEBUG -g)
	$(eval BUILD=$(BUILD)/debug)
	BUILD="$(BUILD)" CFLAGS="$(CFLAGS)" make all

$(BUILD)/src/%.o: src/%.c $(INCS) $(LIB_INCS)
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c -fPIC -I./inc -I./lib -o $@ $<
	@printf "$(BOLD)$$(tput setaf 6)$(notdir $@)$(RESET)"
	@echo "$$(tput setaf 6) ← $?$(RESET)"

$(BUILD)/lib/%.o: lib/%.c $(LIB_INCS)
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c -fPIC -I./lib -o $@ $<
	@printf "$(BOLD)$$(tput setaf 6)$(notdir $@)$(RESET)"
	@echo "$$(tput setaf 6) ← $?$(RESET)"

$(BUILD)/%: $(BUILD)/src/%.o $(patsubst %.c,$(BUILD)/%.o,$(LIB_SRCS))
	@${CC} -o $@ $?;
	@chmod +x $@
	@printf "$(BOLD)$$(tput setaf 4)$(notdir $@)$(RESET)"
	@echo "$$(tput setaf 4) ← $?$(RESET)"

clean:
	rm -rf $(BUILD)

# Special target to zip everything for submission
BRANCH:=$(shell BR=$$(git branch --show-current); echo $$(tr '[:lower:]' '[:upper:]' <<< $${BR:0:1})$${BR:1})
FILE_LIST:=$(shell git ls-tree --full-tree --name-only -r HEAD)
ARCHIVE:=$(AUTHOR)_$(BRANCH)

zip:
	mkdir -p var
	@zip var/$(ARCHIVE).zip $(FILE_LIST)

.PHONY: all debug color_blue color_restore %.bin clean test

.PRECIOUS: $(BUILD)/lib/%.o $(BUILD)/src/%.o

include ./test/Makefile
