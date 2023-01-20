#include "macros.h"
#include "util.h"

int main(int argc, const char *argv[]) {
	char *buf = malloc(BUFFER_SIZE * sizeof(char));
	size_t len;
	// Samples:
	// Subject: C,Meeting   ,01/12/2019,15:30,NEB202
	// Subject: X,Meeting   ,01/12/2019,15:30,NEB202
	while (getline(&buf, &len, stdin) >= 0) {
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
			(*ptr == 0 || *ptr == '\n')
		) printf("%s\n", &buf[9]);
	}
	return 0;
}
