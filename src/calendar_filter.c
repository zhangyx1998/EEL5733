/**
 * EEL5733 Assignment 1
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Calendar Filter 
 */
#include "macros.h"
#include "string.h"
#include "util.h"

int main(int argc, const char *argv[]) {
	char *buf = malloc(BUFFER_SIZE * sizeof(char));
	size_t len;
	// Samples:
	// C,Meeting   ,01/12/2019,15:30,NEB202
	// X,Meeting   ,01/12/2019,15:30,NEB202
	while (getline(&buf, &len, stdin) >= 0) {
		// Avoid unwanted changes to line buffers
		char *ptr = buf;
		// Remove tailing line feed
		while (*ptr != '\n' && *ptr != 0) ptr ++;
		*ptr = 0;
		ptr = &buf[13];
		// Act according to the command
		const char cmd = (const char)*buf;
#ifdef DEBUG
		EPRINT(">> Flag captured: %c\n", cmd);
#endif
		switch (cmd)
		{
		case 'D':
			strcpy(ptr + 11, "--:--,NA        ");
			break;
		case 'C':
		case 'M':
		default:
			break;
		}
		printf("%s\n", ptr);
	}
	return 0;
}
