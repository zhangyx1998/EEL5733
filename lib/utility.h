/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Utility Header
 */
#ifndef UTILITY_H
#define UTILITY_H

#include "macros.h"
#include "stdlib.h"
#include "stdarg.h"

static const char OPTION_END;

#define OPTION_TRAP ((void *)(&OPTION_END))

#define OPTIONAL(...) _util_optional_(0, __VA_ARGS__, OPTION_TRAP)

void *_util_optional_(int l, ...) {
	va_list va;
	void *opt = NULL;
	va_start(va, l);
	while ((opt = va_arg(va, void *)) == NULL);
	va_end(va);
	if (opt != OPTION_TRAP)
		return opt;
	else {
		DEBUG_PRINT("warning: option list empty");
		return NULL;
	}
}

#endif
