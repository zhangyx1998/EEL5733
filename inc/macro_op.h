/**
 * EEL5733 Assignment 3
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Micro Operation Structure Header
 */
#ifndef MACRO_OP_H
#define MACRO_OP_H

#include "stdlib.h"
#include "macros.h"
#include "account.h"

typedef struct MacroOp {
	AccountId src, dst;
	CashValue amount;
} * MacroOp;

static inline MacroOp macroOp(AccountId src, AccountId dst, CashValue amount) {
	MacroOp op = malloc(sizeof(*op));
	op->src = src;
	op->dst = dst;
	op->amount = amount;
	return op;
}

#endif
