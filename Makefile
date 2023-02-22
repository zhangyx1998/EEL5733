# Created by Yuxuan Zhang,
# University of Florida
# ========================
# Env related settings
BUILD    ?= build
# Compiler Parameters
MAKE     := make --no-print-directory
CC       := gcc
LD       := gcc
CCFLAGS  ?= -Wall
LDFLAGS  ?=
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
	$(call report,$@,$<,95)
	@mkdir -p $(shell dirname $@)
	$(CC) $(CCFLAGS) -c -fPIC -I./inc -I./lib -o $@ $<
	@printf "$E0m"

# Complie Library Sources to Objects
$(BUILD)/lib/%.o: lib/%.c $(LIB_INCS) $(BUILD)/env
	$(call report,$@,$<,95)
	@mkdir -p $(shell dirname $@)
	$(CC) $(CCFLAGS) -c -fPIC -I./lib -o $@ $<
	@printf "$E0m"

# Link Executables
$(BUILD)/%: $(BUILD)/src/%.o $(LIB_OBJS) $(OBJS)
	$(call report,$@,$<,94)
	${LD} $(LDFLAGS) -o $@ $^
	@printf "$E0m"; chmod +x $@

define report
@echo "$E0;1;4;$3m$(notdir $1)$E0;$3m <- $2$E0;90m"
endef

include scripts/*.mk

.PHONY: all debug %.debug
.PRECIOUS: $(BUILD)/src/%.o $(BUILD)/lib/%.o 
