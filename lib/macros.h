/**
 * EEL5733 Assignment 2
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Macro Definations 
 */
#ifndef MACROS_H
#define MACROS_H

#include "stdio.h"

#define EPRINT(...) fprintf(stderr, __VA_ARGS__)

#ifdef DEBUG
#define DEBUG_PRINT(...) EPRINT(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) ;
#endif

#define ERR_EXIT(...) {		\
	EPRINT(__VA_ARGS__);	\
	exit(1);				\
}
// Calculates required space for current string to print
#define SPRINTF_SIZE(...) (sizeof(char) * (1 + snprintf(NULL, 0, __VA_ARGS__)))
// Calculates required space and malloc according to space needed
#define MALLOC_PRINTF(PTR, ...)								\
	sprintf(												\
		(PTR = malloc(SPRINTF_SIZE(__VA_ARGS__))),			\
		__VA_ARGS__											\
	)

#endif
