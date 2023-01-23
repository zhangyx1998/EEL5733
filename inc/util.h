/**
 * EEL5733 Assignment 1
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Header for Utility Functions
 */
#include "stdlib.h"
#define BUFFER_SIZE 256
#define LINE_SIZE 28

unsigned int match(char**, const char*);
unsigned int matchChars(char**, const char*);
unsigned int matchDigits(char**, unsigned int);
unsigned int matchContents(char**, unsigned int);
