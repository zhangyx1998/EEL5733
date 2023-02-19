# Created by Yuxuan Zhang,
# University of Florida
# SHELL := /bin/bash
AUTHOR	 ?= Yuxuan_Zhang
# Env related settings
BUILD	 ?= build
# Compiler Parameters
CC		 ?= gcc
CFLAGS	 ?= -Wall
# Source and header file list
SRCS     := $(wildcard src/*.c)
INCS     := $(wildcard inc/*.h)
SRC_LIST := $(patsubst src/%.c,$(BUILD)/src/%.o,$(SRCS))
INC_LIST := $(patsubst inc/%.h,$(BUILD)/src/%.o,$(INCS))
OBJS     := $(filter     $(INC_LIST), $(SRC_LIST))
TARGETS  := $(filter-out $(INC_LIST), $(SRC_LIST))
TARGETS  := $(patsubst $(BUILD)/src/%.o,$(BUILD)/%,$(TARGETS))
# Object list
# Library Lists
LIB_INCS := $(wildcard lib/*.h)
LIB_SRCS := $(wildcard lib/*.c)
LIB_OBJS := $(patsubst %.c,$(BUILD)/%.o,$(LIB_SRCS))
# Default target
all: $(TARGETS)

$(BUILD)/src/%.o: src/%.c $(INCS) $(LIB_INCS)
	$(call report,$@,$<,6)
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c -fPIC -I./inc -I./lib -o $@ $<

$(BUILD)/lib/%.o: lib/%.c $(LIB_INCS)
	$(call report,$@,$<,6)
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c -fPIC -I./lib -o $@ $<

$(BUILD)/%: $(BUILD)/src/%.o $(LIB_OBJS) $(OBJS)
	$(call report,$@,$<,4)
	@${CC} -o $@ $^;
	@chmod +x $@

define report
	@tput sgr0; tput setaf $3; tput bold; tput smul;
	@printf "$(notdir $1)";
	@tput sgr0; tput setaf $3; echo " ← $2"; tput sgr0;
endef

define check_main
	@tput sgr0; tput setaf $3; tput bold; tput smul;
	@printf "$(notdir $1)";
	@tput sgr0; tput setaf $3; echo " ← $2"; tput sgr0;
endef
# Debug mode: outputs to $(BUILD)/debug, add DEBUG to CFLAGS
debug:
	$(eval CFLAGS=$(CFLAGS) -DDEBUG -g)
	$(eval BUILD=$(BUILD)/debug)
	BUILD="$(BUILD)" CFLAGS="$(CFLAGS)" make all

clean:
	rm -rf $(BUILD)
# Supress Makefile's "nothing to be done for ..." message
ENSURE?=all
ensure: $(ENSURE)
	@echo > /dev/null
# Special target to zip everything for submission
BRANCH:=$(shell BR=$$(git branch --show-current); echo $$(tr '[:lower:]' '[:upper:]' <<< $${BR:0:1})$${BR:1})
FILE_LIST:=$(shell git ls-tree --full-tree --name-only -r HEAD)
ARCHIVE:=$(AUTHOR)_$(BRANCH)

zip:
	mkdir -p var
	@zip var/$(ARCHIVE).zip $(FILE_LIST)

include ./test/Makefile

# Declare phony and precious targets
.PHONY: all ensure debug clean test test/%
.PRECIOUS: $(BUILD)/lib/%.o $(BUILD)/src/%.o
