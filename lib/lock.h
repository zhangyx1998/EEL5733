/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Lock Header
 */
#ifndef LOCK_H
#define LOCK_H

#include "pthread.h"

typedef enum {LOCK_RD='R', LOCK_WR='W'} LOCK_ACCESS_TYPE;

#ifdef DEBUG_LIB_LOCK
#define LIB_LOCK_DEBUG_PRINT DEBUG_PRINT
#else
#define LIB_LOCK_DEBUG_PRINT(...)
#endif
/* ================ Mutex Lock Extension ================ */
int _pthread_mutex_trylock_all_(size_t, ...);
#define MUTEX_TRYLOCK_ALL(LIST...) \
	_pthread_mutex_trylock_all_(0, LIST, NULL) 

/* ================ Producer-Consumer Lock Model ================ */
typedef struct PC_Lock {
	pthread_mutex_t *lock;
	pthread_cond_t *sig_produce, *sig_consume;
} * PC_Lock;

PC_Lock create_PC_lock();
void destroy_PC_lock(PC_Lock);

/* ================ Single Write Multiple Read Lock ================ */
typedef void * SWMR_Lock;

SWMR_Lock create_SWMR_lock();
void destroy_SWMR_lock(SWMR_Lock);

void SWMR_lock(SWMR_Lock, const LOCK_ACCESS_TYPE);
void SWMR_unlock(SWMR_Lock);

void SWMR_waitfor(SWMR_Lock, const LOCK_ACCESS_TYPE);

// Mutex lock utility macros
#define MUTEX_LOCK(L) {														\
	const int code = pthread_mutex_lock(L);									\
	ASSERT(code == 0, "error acquiring lock, error code %d", code);			\
	LIB_LOCK_DEBUG_PRINT("[%p] locked", L);							\
}

#define MUTEX_UNLOCK(L) {													\
	const int code = pthread_mutex_unlock(L);								\
	ASSERT(code == 0, "error releasing lock, error code %d", code);			\
	LIB_LOCK_DEBUG_PRINT("[%p] unlocked", L);							\
}

#define MUTEX_COND_WAIT(CONDITION, SIGNAL, LOCK) while (!(CONDITION)) {		\
	const int code = pthread_cond_wait(SIGNAL, LOCK);						\
	ASSERT(code == 0, "error receiving signal, error code %d", code);		\
}

#endif
