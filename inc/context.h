/**
 * EEL5733 Assignment 3
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Execution Context Structure Header
 */
#ifndef CONTEXT_H
#define CONTEXT_H

#include "../lib/vector.h"
#include "../lib/stream.h"
#include "../lib/lock.h"
#include "parser.h"
#include "account.h"

typedef struct Context {
	// Stream passing instructions to workers
	Stream inst_stream;
	// Vector to store all accounts
	SWMR_Lock account_vec_lock;
	Vector account_vec;
} * Context;


Context context() {
	Context c = malloc(sizeof(*c));
	c->inst_stream = stream();
	c->account_vec = vector();
	c->account_vec_lock = create_SWMR_lock();
	return c;
}

void _context(Context c) {
	_stream(c->inst_stream);
	_vector(c->account_vec);
	destroy_SWMR_lock(c->account_vec_lock);
	free(c);
}

#endif
