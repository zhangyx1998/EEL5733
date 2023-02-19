/**
 * EEL5733 Assignment 3
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Account Data Structure Header
 */
#ifndef PARSER_H
#define PARSER_H

#include "account.h"

typedef struct {
	enum {
		INS_NONE = 'N',
		INS_CREATE = 'C',
		INS_TRANSFER = 'T'
	} type;
	AccountId account_src, account_dst;
	CashValue amount;
} * Instruction;

Instruction parse(const char * const);

#endif
