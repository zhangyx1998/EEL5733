// Force enable debug mode for debug logs
#define DEBUG
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "macros.h"

int main(int argc, char *argv[]) {
	const char *file = argc > 1 ? argv[1] : "/dev/tux0";
	int fd = open(file, O_RDWR);
	ASSERT(fd >= 0, "Unable to open file %s (%d)", file, fd);
	DEBUG_PRINT("Successfully opened: %s", file);
	// Create readline buffer
	char *buf = NULL; size_t cap = 0;
	while (getline(&buf, &cap, stdin) != EOF) {
		write(fd, buf, (strlen(buf)) * sizeof(char));
	}
	DEBUG_PRINT("Finished writing to device: %s", file);
	DEBUG_PRINT("Reading from device: %s", file);
	ssize_t read_size;
	while ((read_size = read(fd, buf, cap)) > 0) {
		printf("%.*s", (int)read_size, buf);
	}
	ASSERT(close(fd) == 0, "Error closing file %s", file);
	DEBUG_PRINT("Successfully closed device: %s", file);
	return 0;
}
