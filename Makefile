# Object list
OBJS = main.o
# C++ Compiling Parameters
CC = gcc
CFLAGS = -Wall -ansi -g
# Env related settings
BUILD_PATH = build
# Set up C suffixes & relationship between .cpp and .o files
$(BUILD_PATH):
	@mkdir -p $(BUILD_PATH)

.SUFFIXES: .c

%.o: src/%.c
	$(CC) $(CFLAGS) -c -I./inc -o $(BUILD_PATH)/$@ $<

.cpp.o: $(BUILD_PATH)
	$(CC) $(CFLAGS) -c ./src/$< -I ./inc -o $(BUILD_PATH)/$<.cpp.o

# Change console output color
color_gray:
	@echo "\033[1;30m"
color_restore:
	@echo "\033[0m"

run: color_gray fabric color_restore
	@$(BUILD_PATH)/app

fabric: $(BUILD_PATH) $(OBJS)
	${CC} -o $(BUILD_PATH)/app $(BUILD_PATH)/*.o;
	@chmod +x $(BUILD_PATH)/app

clean:
	rm -rf $(BUILD_PATH)

.PHONY: clean $(BUILD_PATH)
