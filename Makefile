# Object list
OBJS = util.o
# Compiler Parameters
CC = gcc
CFLAGS = -Wall
# Env related settings
BUILD_PATH = build
# Default target
all: $(BUILD_PATH) color_gray email_filter.bin calendar_filter.bin location_updater.bin color_restore
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

run:
	$(BUILD_PATH)/email_filter.bin < test/1.in | $(BUILD_PATH)/calendar_filter.bin

# Change console output color
color_gray:
	@echo "\033[1;30m"
color_restore:
	@echo "\033[0m"

clean:
	rm -rf $(BUILD_PATH)

.PHONY: clean $(BUILD_PATH) test

# include ./test/Makefile