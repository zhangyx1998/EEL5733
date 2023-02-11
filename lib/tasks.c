/**
 * EEL5733 Assignment 2
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Tasks implementation
 */
#include "macros.h"
#include "string.h"
#include "list.h"
#include "util.h"

void email_filter(const struct IO * const io) {
	char *buf = NULL;
	size_t len = 0;
	// Samples:
	// Subject: C,Meeting   ,01/12/2019,15:30,NEB202
	// Subject: X,Meeting   ,01/12/2019,15:30,NEB202
	while (io->getline(&buf, &len) >= 0) {
		// Avoid unwanted changes to line buffers
		char *ptr = buf;
		// Remove tailing line feed
		while (*ptr != '\n' && *ptr != 0) ptr ++;
		*ptr = 0;
		ptr = buf;
		// Validate input
		if (
			match			(&ptr, "Subject: ") &&
			// Operation
			matchChars		(&ptr, "CDX") 		&&
			matchChars		(&ptr, ",") 		&&
			// Title
			matchContents	(&ptr, 10) 			&&
			matchChars		(&ptr, ",") 		&&
			// "MM/DD/YYYY"
			matchDigits		(&ptr, 2) 			&&
			matchChars		(&ptr, "/") 		&&
			matchDigits		(&ptr, 2) 			&&
			matchChars		(&ptr, "/") 		&&
			matchDigits		(&ptr, 4) 			&&
			matchChars		(&ptr, ",") 		&&
			// "HH:MM"
			matchDigits		(&ptr, 2) 			&&
			matchChars		(&ptr, ":") 		&&
			matchDigits		(&ptr, 2) 			&&
			matchChars		(&ptr, ",") 		&&
			// Subject
			matchContents   (&ptr, 10)          &&
			// End of line
			(*ptr == 0 || *ptr == '\n')			&&
			// Set line end to 0
			((*ptr = 0) == 0)
		) io->puts(&buf[9]);
	}
}

void calendar_filter(const struct IO * const io) {
	char *buf = NULL;
	size_t len = 0;
	// Samples:
	// C,Meeting   ,01/12/2019,15:30,NEB202
	// X,Meeting   ,01/12/2019,15:30,NEB202
	while (io->getline(&buf, &len) >= 0) {
		// Avoid unwanted changes to line buffers
		char *ptr = buf;
		// Remove tailing line feed
		while (*ptr != '\n' && *ptr != 0) ptr ++;
		*ptr = 0;
		ptr = &buf[2];
		// Act according to the command
		const char cmd = (const char)*buf;
		switch (cmd)
		{
		case 'C':
			list_create(parse(ptr), io);
			break;
		case 'D':
			list_delete(parse(ptr), io);
			break;
		case 'X':
			list_modify(parse(ptr), io);
			break;
		default:
			break;
		}
	}
}
