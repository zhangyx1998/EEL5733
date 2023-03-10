# Created by Yuxuan Zhang,
# University of Florida
# ========================
# Env related settings
BUILD    ?= build
# Compiler Parameters
MAKE     := make --no-print-directory
CC       := gcc
LD       := gcc
CFLAGS   ?=
CFLAGS   := $(CFLAGS) -Wall -c
LDFLAGS  ?= -lpthread -pthread
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

# Complie User Sources to Objects
$(BUILD)/src/%.o: src/%.c $(INCS) $(LIB_INCS) $(BUILD)/env
	@mkdir -p $(shell dirname $@)
	$(eval CMD:=$(CC) $(CFLAGS) -I./inc -I./lib -o $@ $<)
	@$(call report,$@,$<,95,$(CMD))
	@$(CMD);

# Complie Library Sources to Objects
$(BUILD)/lib/%.o: lib/%.c $(LIB_INCS) $(BUILD)/env
	@mkdir -p $(shell dirname $@)
	$(eval CMD:=$(CC) $(CFLAGS) -I./lib -o $@ $<)
	@$(call report,$@,$<,95,$(CMD))
	@$(CMD);

# Link Executables
$(BUILD)/%: $(BUILD)/src/%.o $(LIB_OBJS) $(OBJS)
	$(eval CMD:=${LD} $(LDFLAGS) -o $@ $^)
	@$(call report,$@,$<,94,$(CMD))
	@$(CMD);
	@chmod +x $@
# Thread safe report (-j compatible)
define report
echo "$E0;1;4;$3m$(notdir $1)$E0;$3m <- $2\n$E0;90m$(CMD)$E0m"
endef

include scripts/*.mk

.PHONY: all debug %.debug
.PRECIOUS: $(BUILD)/src/%.o $(BUILD)/lib/%.o 
