/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Header for Utility Functions
 */
#ifndef UTIL_H
#define UTIL_H

#include "stdlib.h"
#include "stdio.h"
#define BUFFER_SIZE 256
#define LINE_SIZE 28

struct IO {
	int (*puts)(const char *);
	ssize_t (*getline)(char **, size_t *);
};

ssize_t stdin_getline(char **, size_t *);

unsigned int match(char**, const char*);
unsigned int matchChars(char**, const char*);
unsigned int matchDigits(char**, unsigned int);
unsigned int matchContents(char**, unsigned int);

#endif
