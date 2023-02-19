/**
 * EEL5733 Assignment 3
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Parser Implementations
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "macros.h"
#include "parser.h"

#define IS_SPACE(S) ((S) == ' ' || (S) == '\t' || (S) =='\n')

const char *next(const char *s) {
	while (IS_SPACE(*s)) s++;
	return s;
}

unsigned int matchNumbers(const char * const str, const char ** const v) {
	const char *s = next(str), *p;
	unsigned int count = 0;
	while (*s) {
		p = s;
		while ((*p) >= '0' && (*p) <= '9') p++;
		if (!(*p) || IS_SPACE(*p)) {
			v[count++] = s;
			s = next(p);
		} else break;
	}
	return count;
}

int match(const char **s, const char * const template) {
	const char *a = *s, *b = template;
	while ((*b) && ((*a) == (*b))) { a++; b++; }
	if ((*b) == 0) {
		*s = a;
		return 1;
	} else {
		return 0;
	}
}

static inline const char * removeLF(const char * s) {
	size_t l = strlen(s);
	while (l > 0 && s[l - 1] == '\n') l--;
	char * str = malloc(l + 1);
	memcpy(str, s, l);
	str[l] = 0;
	return str;
}

#define VEC_SIZE 5
Instruction parse(const char * const buf) {
	const char
		*const s = removeLF(buf),
		*str = s,
		*v[VEC_SIZE];
	Instruction inst = malloc(sizeof(Instruction));
	// Try to validate string as a transfer instruction
	if (match(&str, "Transfer") && matchNumbers(str, v) == 3) {
		inst->type        = INS_TRANSFER;
		inst->account_src = (AccountId)atoi(v[0]);
		inst->account_dst = (AccountId)atoi(v[1]);
		inst->amount      = (CashValue)atoi(v[2]);
	} else if (matchNumbers(str, v) == 2) {
		inst->type        = INS_CREATE;
		inst->account_src = (AccountId)atoi(v[0]);
		inst->account_dst = (AccountId)atoi(v[0]);
		inst->amount      = (CashValue)atoi(v[1]);
	} else {
		// No match, log the error and return empty instruction
		EPRINT("unable to match \"%s\" with any instruction type.", str);
		inst->type        = INS_NONE;
		inst->account_src = 0;
		inst->account_dst = 0;
		inst->amount      = 0;
	}
	DEBUG_PRINT(
		"[%c] account_src=%-2u account_dst=%-2u amount=%lu",
		(char)inst->type,
		inst->account_src,
		inst->account_dst,
		inst->amount
	);
	free((void *)s);
	return inst;
}
