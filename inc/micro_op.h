/**
 * EEL5733 Assignment 3
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Micro Operation Structure Header
 */
#ifndef MICRO_OP_H
#define MICRO_OP_H

#include "stdlib.h"
#include "macros.h"
#include "account.h"

typedef enum MicroOpType {OP_CREATE='$', OP_SEND='-', OP_RECV='+'} MicroOpType;

typedef struct MicroOp {
	MicroOpType type;
	AccountId id;
	CashValue amount;
} * MicroOp;

static inline MicroOp microOp(MicroOpType type, AccountId id, CashValue amount) {
	MicroOp op = malloc(sizeof(*op));
	op->type = type;
	op->id = id;
	op->amount = amount;
	DEBUG_PRINT("account<%u> %c%lu", id, (char)type, amount);
	return op;
}

#endif
