/**
 * EEL5733 Assignment Project
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief string matching utilities 
 */
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

unsigned int match(char** src, const char* template) {
#ifdef DEBUG
	EPRINT("DEBUG MESSAGE: match(\"%s\", \"%s\")\n", *src, template);
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
	EPRINT("DEBUG MESSAGE: matchChars(\'%c\', \"%s\")\n", **src, template);
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
	EPRINT("DEBUG MESSAGE: matchDigits(\"%s\", %d)\n", *src, n);
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
	EPRINT("DEBUG MESSAGE: matchContents(\"%s\", %d)\n", *src, n);
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
