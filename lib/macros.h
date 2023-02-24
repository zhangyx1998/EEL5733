/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Macro Definations
 */
#ifndef MACROS_H
#define MACROS_H

#include "stdio.h"
#include "unistd.h"
#include "ANSI.h"
// Reports count of __VA_ARGS__
#define NUM_ARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
// Calculates required space for current string to print
#define SPRINTF_SIZE(...) \
	((1 + snprintf(NULL, 0, __VA_ARGS__)) * sizeof(char))
// Calculates required space and malloc according to space needed
#define ALLOC_SPRINTF(S, ...) {									\
	const size_t _ ## S ## _size_ = SPRINTF_SIZE(__VA_ARGS__);	\
	char *_ ## S ## _ = malloc(_S_);							\
	sprintf(													\
		_ ## S ## _,											\
		__VA_ARGS__												\
	);															\
	S = realloc(S, (_ ## S ## _size_) * sizeof(char));			\
	memcpy(S, _ ## S ## _, _ ## S ## _size_);					\
	free(_ ## S ## _);											\
}

#define TRACE(T)							\
	"%s:%d->%s"" "T,						\
	__FILE__, __LINE__, __FUNCTION__

#define COLOR_TRACE(T)						\
	S_DIM("%s:%d "S_UNDERLINE("%s"))" "T,	\
	__FILE__, __LINE__, __FUNCTION__

#define TRACE_COLOR_PRINT(F, C, T, ...) (	\
	isatty(fileno(F))						\
		? fprintf(							\
			F,								\
			COLOR_TRACE(C T RESET),			\
			__VA_ARGS__						\
		)									\
		: fprintf(							\
			F,								\
			TRACE(T),						\
			__VA_ARGS__						\
		),									\
	fflush(F)								\
)

#define _EPRINT(T, ...)	\
	TRACE_COLOR_PRINT(stderr, C_RED, T"%c", __VA_ARGS__)
#define EPRINT(...) _EPRINT(__VA_ARGS__, '\n')

#define _DEBUG_PRINT(T, ...) \
	TRACE_COLOR_PRINT(stderr, C_CYAN, T"%c", __VA_ARGS__)

#ifdef DEBUG
#define DEBUG_PRINT(...) _DEBUG_PRINT(__VA_ARGS__, '\n')
#else
#define DEBUG_PRINT(...);
#endif

#ifdef DEBUG_TRACE
#define TR(F) \
	(TRACE_COLOR_PRINT(stderr, C_YELLOW, #F"%c", '\n'), F)
#else
#define TR(F) F
#endif

#define ERR_EXIT(CODE, ...) {	\
	EPRINT(__VA_ARGS__);		\
	exit(CODE);					\
}

#define ASSERT(COND, ...) if (!(COND)) ERR_EXIT(1, __VA_ARGS__);

#endif
