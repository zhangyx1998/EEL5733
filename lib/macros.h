/**
 * EEL5733 Assignment 2
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Macro Definations 
 */
#ifndef MACROS_H
#define MACROS_H

#include "stdio.h"
#include "unistd.h"

#define COLOR_PRINT(F, C, ...) {		\
	const int flag = isatty(fileno(F));	\
	if (flag) fputs(C, F);				\
	fprintf(F, __VA_ARGS__);			\
	if (flag) fputs("\e[0m", F);		\
}

#define TRACE(F) { 						\
	COLOR_PRINT(						\
		F, "\e[38;5;235m",				\
		"%s:%-3d ", __FILE__, __LINE__	\
	);									\
	COLOR_PRINT(						\
		F, "\e[38;5;235m\e[4m",			\
		"%s", __FUNCTION__				\
	);									\
}

#define EPRINT(...) {		\
	TRACE(stderr);			\
	putc(' ', stderr);		\
	COLOR_PRINT(			\
		stderr, "\e[31m",	\
		__VA_ARGS__			\
	);						\
	putc('\n', stderr);		\
}

#ifdef DEBUG
#define DEBUG_PRINT(...) {	\
	TRACE(stderr);			\
	putc(' ', stderr);		\
	COLOR_PRINT(			\
		stderr, "\e[36m",	\
		__VA_ARGS__			\
	);						\
	putc('\n', stderr);		\
}
#else
#define DEBUG_PRINT(...);
#endif

#define ERR_EXIT(...) {		\
	EPRINT(__VA_ARGS__);	\
	exit(1);				\
}

#define ASSERT(COND, ...) if (!(COND)) ERR_EXIT(__VA_ARGS__);
// Calculates required space for current string to print
#define SPRINTF_SIZE(...) \
	(sizeof(char) * (1 + snprintf(NULL, 0, __VA_ARGS__)))
// Calculates required space and malloc according to space needed
#define MALLOC_PRINTF(PTR, ...)								\
	sprintf(												\
		(PTR = malloc(SPRINTF_SIZE(__VA_ARGS__))),			\
		__VA_ARGS__											\
	)

#endif
