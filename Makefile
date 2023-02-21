# Created by Yuxuan Zhang,
# University of Florida
# SHELL := /bin/bash
AUTHOR   ?= Yuxuan_Zhang
# Env related settings
BUILD    ?= build
# Compiler Parameters
CC       ?= gcc
CFLAGS   ?= -Wall
# Source and header file list
SRCS     := $(wildcard src/*.c)
INCS     := $(wildcard inc/*.h)
SRC_LIST := $(patsubst src/%.c,$(BUILD)/src/%.o,$(SRCS))
INC_LIST := $(patsubst inc/%.h,$(BUILD)/src/%.o,$(INCS))
OBJS     := $(filter     $(INC_LIST), $(SRC_LIST))
TARGETS  := $(filter-out $(INC_LIST), $(SRC_LIST))
TARGETS  := $(patsubst $(BUILD)/src/%.o,$(BUILD)/%,$(TARGETS))
# Library Lists
LIB_INCS := $(wildcard lib/*.h)
LIB_SRCS := $(wildcard lib/*.c)
LIB_OBJS := $(patsubst %.c,$(BUILD)/%.o,$(LIB_SRCS))
# Format Escape Sequence
E        :=\033[
# Default target
all: $(TARGETS)

$(BUILD)/src/%.o: src/%.c $(INCS) $(LIB_INCS)
	$(call report,$@,$<,6)
	@mkdir -p $(shell dirname $@)
	@tput setaf 0
	$(CC) $(CFLAGS) -c -fPIC -I./inc -I./lib -o $@ $<
	@tput sgr0;

$(BUILD)/lib/%.o: lib/%.c $(LIB_INCS)
	$(call report,$@,$<,6)
	@mkdir -p $(shell dirname $@)
	@tput setaf 0
	$(CC) $(CFLAGS) -c -fPIC -I./lib -o $@ $<
	@tput sgr0;

$(BUILD)/%: $(BUILD)/src/%.o $(LIB_OBJS) $(OBJS)
	$(call report,$@,$<,4)
	@tput setaf 0
	${CC} -o $@ $^
	@tput sgr0;
	@chmod +x $@

define report
	@tput sgr0; tput setaf $3; tput bold; tput smul;
	@printf "$(notdir $1)";
	@tput sgr0; tput setaf $3; echo " ← $2"; tput sgr0;
endef

# Debug mode: outputs to $(BUILD)/debug, add DEBUG to CFLAGS
debug:
	$(eval CFLAGS += -DDEBUG -g)
	$(eval BUILD  := $(BUILD)/debug)
	@	BUILD="$(BUILD)" CFLAGS="$(CFLAGS)"			\
		make ensure --no-print-directory

%.debug: debug
	@rm -f $(BUILD)/$(@:.debug=.o)
	$(eval DBG_T :=$(shell	\
		echo $(@:.debug=) |	\
		sed -e 's/\//_/g' |	\
		tr 'a-z' 'A-Z'		\
	))
	$(eval CFLAGS += -DDEBUG_$(DBG_T))

# Supress Makefile's "nothing to be done for ..." message
ENSURE?=all
ensure: $(ENSURE)
	@echo > /dev/null

# Special target to zip everything for submission
BRANCH   :=$(shell git branch --show-current)
BRANCH   :=$(patsubst assignment_%,Assignment_%,$(BRANCH))
FILE_LIST:=$(shell git ls-tree --full-tree --name-only -r HEAD)
ARCHIVE  :=var/$(AUTHOR)_$(BRANCH).zip
MSG_ZIP  :=$$(tput setaf 9)Commit all changes before $$(tput smul)zip$$(tput rmul)!$$(tput sgr0)
zip:
	@	git update-index --refresh		\
	&&	git diff-index --quiet HEAD --	\
	||	(echo $(MSG_ZIP); exit 1)
	@	mkdir -p $(shell dirname $(ARCHIVE))
	@	zip $(ARCHIVE) $(FILE_LIST) 1> /dev/null
	@-	zipinfo -1 $(ARCHIVE) | tree --fromfile .

include ./test/Makefile

clean:
	rm -rf $(BUILD)
	rm -rf $(ARCHIVE)
# Declare phony and precious targets
.PHONY:				\
	all clean zip	\
	ensure %.ensure	\
	debug %.debug	\
	test test/%

.PRECIOUS: $(BUILD)/lib/%.o $(BUILD)/src/%.o
