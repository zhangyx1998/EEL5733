# Object list
OBJS = main.o
# Compiler Parameters
CC = gcc
CFLAGS = -Wall
# Check for debug flag
debug: CFLAGS += -DDEBUG -g
# Env related settings
BUILD_PATH = build
# Default target
compose: color_gray bin color_restore
# Set up C suffixes & relationship between .cpp and .o files
$(BUILD_PATH):
	@mkdir -p $(BUILD_PATH)

.SUFFIXES: .c

%.o: src/%.c
	$(CC) $(CFLAGS) -c -I./inc -o $(BUILD_PATH)/$@ $<

.cpp.o: $(BUILD_PATH)
	$(CC) $(CFLAGS) -c ./src/$< -I ./inc -o $(BUILD_PATH)/$<.cpp.o

bin: $(BUILD_PATH) $(OBJS)
	${CC} -o $(BUILD_PATH)/app $(BUILD_PATH)/*.o;
	@chmod +x $(BUILD_PATH)/app

# Change console output color
color_gray:
	@echo "\033[1;30m"
color_restore:
	@echo "\033[0m"

run: fabric
	@$(BUILD_PATH)/app

clean:
	rm -rf $(BUILD_PATH)

.PHONY: clean $(BUILD_PATH) test

include ./test/Makefile