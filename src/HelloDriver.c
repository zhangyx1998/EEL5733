// Force enable debug mode for debug logs
#define DEBUG
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "macros.h"

int main(int argc, char *argv[]) {
	const char *file = argc > 1 ? argv[1] : "/dev/tux0";
	int fd = -1;
	fd = open(file, O_WRONLY);
	ASSERT(fd >= 0, "Unable to open file %s (%d, WRONLY)", file, fd);
	DEBUG_PRINT("Successfully opened: %s", file);
	// Create readline buffer
	char *buf = NULL; size_t cap = 0;
	while (getline(&buf, &cap, stdin) != EOF) {
		write(fd, buf, (strlen(buf)) * sizeof(char));
	}
	DEBUG_PRINT("Finished writing to device: %s", file);

	// Reopen file in read only mode
	close(fd); fd = open(file, O_RDONLY);
	ASSERT(fd >= 0, "Unable to open file %s (%d, RDONLY)", file, fd);

	DEBUG_PRINT("Reading from device: %s", file);
	ssize_t read_size;
	while ((read_size = read(fd, buf, cap)) > 0) {
		for (size_t i = 0; i < read_size; i++) {
			if (buf[i]) putchar(buf[i]);
			else goto READ_FINISH;
		}
	}
READ_FINISH:
	ASSERT(close(fd) == 0, "Error closing file %s", file);
	DEBUG_PRINT("Successfully closed device: %s", file);
	return 0;
}
