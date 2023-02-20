/**
 * EEL5733 Assignment 3
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Electronic Found Transfer
 * ==========================================================
 * Design Concepts:
 * 
 * 1. A global "Single Write Multiple Read Lock" is placed on
 *    the account list. The lock will allow multiple threads
 *    to read the list at the same time, but only allows one
 *    thread to write the list exclusively.
 * 
 *    When this lock is in "WRITE" state, no threads can gain
 *    access to the lock even as a reader.
 * 
 * 2. Each element in the list has its own mutex lock, which
 *    guards only the `balance` field.
 * 
 * 3. Worker threads will grab parsed instructions and try to
 *    locate the accounts using `id`. If no match was found,
 *    the thread will wait for next write to the list and try
 *    again.
 */

#include "stdio.h"
#include "pthread.h"
/* ======= project library ====== */
#include "vector.h"
#include "lock.h"
#include "macros.h"
/* ======= local includes ======= */
#include "micro_op.h"
#include "account.h"
#include "context.h"
#include "parser.h"

void *producer(Context ctx) {
	char *buf = NULL;
	size_t capacity = 0;
	while (getline(&buf, &capacity, stdin) >= 0) {
		Instruction inst = parse(buf);
		if (inst->type == INS_CREATE) {
			SWMR_lock(ctx->account_vec_lock, LOCK_WR);
			vector_push(ctx->account_vec, account(inst->account_src, inst->amount));
			SWMR_unlock(ctx->account_vec_lock);
		} else if (inst->type == INS_TRANSFER) {
			stream_write(
				ctx->inst_stream,
				(StreamElement)microOp(OP_SEND, inst->account_src, inst->amount)
			);
			stream_write(
				ctx->inst_stream,
				(StreamElement)microOp(OP_RECV, inst->account_dst, inst->amount)
			);
		} else {
			DEBUG_PRINT("got instruction [%p] with unknown type", (void *)inst);
		}
		free((void *)inst);
	}
	// Close stream
	stream_write(ctx->inst_stream, END_OF_STREAM);
	return NULL;
}

// Locate the account from variable vector
static inline Account getAccount(Context ctx, AccountId id) {
	size_t i = 0;
	while (1) {
		while (i >= vector_len(ctx->account_vec)) {
			DEBUG_PRINT("waiting for account<%u> to be created", id);
			SWMR_waitfor(ctx->account_vec_lock, LOCK_WR);
		}
		Account a = vector_get(ctx->account_vec, i);
		if (a->id == id) {
			return a;
		}
		else i++;
	}
}

void *worker(void * context) {
	DEBUG_PRINT("initiated");
	const Context ctx = context;
	MicroOp op;
	while ((op = (MicroOp)stream_read(ctx->inst_stream)) != (MicroOp)END_OF_STREAM) {
		SWMR_lock(ctx->account_vec_lock, LOCK_RD);
		Account a = getAccount(ctx, op->id); {
			// Acquire lock for this account
			if (pthread_mutex_trylock(a->mutex) == 0) {
				// Do the transfer
				switch (op->type) {
					case OP_RECV:
						DEBUG_PRINT(
							"account<%u> +%lu (balance %ld)",
							op->id, op->amount, a->balance
						);
						a->balance += op->amount;
						break;
					case OP_SEND:
						DEBUG_PRINT(
							"account<%u> -%lu (balance %ld)",
							op->id, op->amount, a->balance
						);
						a->balance -= op->amount;
						break;
					default:
						EPRINT("unknown operation %d", (int)op->type);
				}
				free(op);
				MUTEX_UNLOCK(a->mutex);
			} else {
				// Push micro operation back to stream
				stream_write(ctx->inst_stream, (StreamElement)op);
			}
		}
		SWMR_unlock(ctx->account_vec_lock);
	}
	return NULL;
}

int main(int argc, const char *argv[]) {
	// Redirect stdin if given file path from command line
	if (argc >= 2) freopen(argv[1], "r", stdin);
	// Parse optional command line argument
	const size_t
		n_threads = (size_t)(argc >= 3 ? atoi(argv[2]) : 10),
		buf_size  = (size_t)(argc >= 4 ? atoi(argv[3]) : 16);
	// Check if n_threads > 0
	ASSERT(n_threads > 0, "number of threads must be greater than 0");
	// Initialize context
	Context ctx = context(buf_size);
	// Launch execution workers
	pthread_t *tid = malloc(n_threads * sizeof(*tid));
	for (size_t i = 0; i < n_threads; i++)
		pthread_create(tid + i, NULL, worker, ctx);
	// Instruction producer loop
	producer(ctx);
	// Wait for all thread to end
	for (size_t i = 0; i < n_threads; i++) {
		pthread_join(tid[i], NULL);
		DEBUG_PRINT("thread <%zu> joined to main", i);
	}
	// Print final result
	while (vector_len(ctx->account_vec)) {
		Account a = vector_unshift(ctx->account_vec);
		printf("%u %ld\n", a->id, a->balance);
		_account(a);
	}
	// Free context
	_context(ctx);
	return 0;
}
