/**
 * EEL5733 Assignment 2
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief String Matching Utilities 
 */
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"

ssize_t stdin_getline(char **buf, size_t *len) {
	return getline(buf, len, stdin);
}

unsigned int match(char** src, const char* template) {
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
	while (n-- > 0) {
		if (**src == '\0' || **src == ',') {
			(*src) ++;
			return 0;
		}
		else (*src) ++;
	}
	return 1;
}
