/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Lock implementations
 */
#include "stdlib.h"
#include "pthread.h"
#include "lock.h"
#include "macros.h"

/* ================ Producer-Consumer Lock Model ================ */
PC_Lock create_PC_lock() {
	PC_Lock l = malloc(sizeof(*l));
	l->lock = malloc(sizeof(*(l->lock)));
	pthread_mutex_init(l->lock, NULL);
	l->sig_produce = malloc(sizeof(*(l->sig_produce)));
	l->sig_consume = malloc(sizeof(*(l->sig_consume)));
	pthread_cond_init(l->sig_produce, NULL);
	pthread_cond_init(l->sig_consume, NULL);
	return l;
}

void destroy_PC_lock(PC_Lock l) {
	// Destroy internal allocations
	pthread_mutex_destroy(l->lock);
	pthread_cond_destroy(l->sig_produce);
	pthread_cond_destroy(l->sig_consume);
	// Free internal pointers
	free(l->lock);
	free(l->sig_produce);
	free(l->sig_consume);
	// Free the lock struct
	free(l);
}

/* ================ Single Write Multiple Read Lock ================ */
// Private Struct Definition
typedef struct SWMR_Lock {
	// State variables
	LOCK_ACCESS_TYPE state;
	unsigned int ref_count;
	// Lock variables
	pthread_mutex_t *lock;
	pthread_cond_t *signal;
} * _SWMR_Lock_;

SWMR_Lock create_SWMR_lock() {
	_SWMR_Lock_ l = malloc(sizeof(*l));
	l->state = LOCK_RD;
	l->ref_count = 0;
	l->lock = malloc(sizeof(*(l->lock)));
	l->signal = malloc(sizeof(*(l->signal)));
	pthread_mutex_init(l->lock, NULL);
	pthread_cond_init(l->signal, NULL);
	return l;
}

void destroy_SWMR_lock(SWMR_Lock lock) {
	const _SWMR_Lock_ l = (_SWMR_Lock_)lock;
	// Destroy internal allocations
	pthread_mutex_destroy(l->lock);
	pthread_cond_destroy(l->signal);
	// Free internal pointers
	free(l->lock);
	free(l->signal);
	// Free the lock struct
	free(l);
}

#ifdef DEBUG_LIB_LOCK
#define REPORT_SWMR_LOCK_STATE(L) 	\
	DEBUG_PRINT(					\
		"[%c] (%u refs)",			\
		(char)(L)->state,			\
		(L)->ref_count				\
	)
#else
#define REPORT_SWMR_LOCK_STATE(L)
#endif

void SWMR_lock(SWMR_Lock lock, const LOCK_ACCESS_TYPE t) {
	const _SWMR_Lock_ l = (_SWMR_Lock_)lock;
	MUTEX_LOCK(l->lock);
	// Update lock status
	if (t == LOCK_WR) {
		MUTEX_COND_WAIT(
			l->state == LOCK_RD && l->ref_count == 0,
			l->signal, l->lock
		);
		// Set lock state to WRITE
		l->state = t;
	} else {
		MUTEX_COND_WAIT(
			l->state == LOCK_RD,
			l->signal, l->lock
		);
		// Increment ref_count
		l->ref_count++;
	}
	REPORT_SWMR_LOCK_STATE(l);
	MUTEX_UNLOCK(l->lock);
}

void SWMR_unlock(SWMR_Lock lock) {
	const _SWMR_Lock_ l = (_SWMR_Lock_)lock;
	MUTEX_LOCK(l->lock);
	// Check for current state
	if (l->state == LOCK_WR) {
		l->state = LOCK_RD;
		pthread_cond_broadcast(l->signal);
	} else {
		ASSERT(l->ref_count > 0, "releasing a free lock");
		if (--(l->ref_count) == 0)
			pthread_cond_broadcast(l->signal);
	}
	REPORT_SWMR_LOCK_STATE(l);
	MUTEX_UNLOCK(l->lock);
}

void SWMR_waitfor(SWMR_Lock lock, const LOCK_ACCESS_TYPE t) {
	const _SWMR_Lock_ l = (_SWMR_Lock_)lock;
	MUTEX_LOCK(l->lock);
	if (t == LOCK_WR) {
		MUTEX_COND_WAIT(
			l->state == t, l->signal, l->lock
		);
	} else {
		MUTEX_COND_WAIT(
			l->state == t && l->ref_count > 0,
			l->signal, l->lock
		);
	}
	// Wait for the next signal which indicates completion of state
	MUTEX_COND_WAIT(1, l->signal, l->lock);
	MUTEX_UNLOCK(l->lock);
	REPORT_SWMR_LOCK_STATE(l);
}
