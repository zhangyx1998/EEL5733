/**
 * EEL5733 Assignment Project
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief match 
 */
#include "main.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "main.h"

unsigned int match(char** src, const char* template) {
#ifdef DEBUG
	printf("DEBUG MESSAGE: match(\"%s\", \"%s\")\n", *src, template);
#endif
	while (*template != '\0') {
		if (**src != *template) {
			(*src) ++;
			return 0;
		}
		else (*src) ++;
		template ++;
	}
	return 1;
}

unsigned int matchChars(char** src, const char* template) {
#ifdef DEBUG
	printf("DEBUG MESSAGE: matchChars(\'%c\', \"%s\")\n", **src, template);
#endif
	while (*template != '\0') {
		if (**src == *template) {
			(*src) ++;
			return 1;
		}
		template ++;
	}
	(*src) ++;
	return 0;
}

unsigned int matchDigits(char** src, unsigned int n) {
#ifdef DEBUG
	printf("DEBUG MESSAGE: matchDigits(\"%s\", %d)\n", *src, n);
#endif
	while (n-- > 0) {
		if (**src < '0' || **src > '9') {
			(*src) ++;
			return 0;
		}
		else (*src) ++;
	}
	return 1;
}

unsigned int matchContents(char** src, unsigned int n) {
#ifdef DEBUG
	printf("DEBUG MESSAGE: matchContents(\"%s\", %d)\n", *src, n);
#endif
	while (n-- > 0) {
		if (**src == '\0' || **src == ',') {
			(*src) ++;
			return 0;
		}
		else (*src) ++;
	}
	return 1;
}

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
