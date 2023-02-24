/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Stream implementations
 */
#include "pthread.h"
#include "lock.h"
#include "vector.h"
#include "stream.h"
#include "macros.h"

typedef struct {
	Vector vector;
	int closed;
	PC_Lock mutex;
} * _Stream_;

Stream stream() {
	_Stream_ s = malloc(sizeof(*s));
	s->vector = vector();
	s->closed = 0;
	s->mutex = create_PC_lock();
	return s;
}

void _stream(Stream stream) {
	const _Stream_ s = stream;
	_vector(s->vector);
	destroy_PC_lock(s->mutex);
	free((void *)s);
}

void stream_write(Stream stream, StreamElement e) {
	const _Stream_ s = stream;
	MUTEX_LOCK(s->mutex->lock);
	// Check if writing EOS to stream
	if (e == END_OF_STREAM) {
		MUTEX_COND_WAIT(
			vector_len(s->vector) == 0,
			s->mutex->sig_consume,
			s->mutex->lock
		);
		s->closed = 1;
		DEBUG_PRINT("stream [%p] closed", (void *)s);
	} else {
		vector_push(s->vector, e);
	}
	pthread_cond_broadcast(s->mutex->sig_produce);
	MUTEX_UNLOCK(s->mutex->lock);
}

StreamElement stream_read(Stream stream) {
	const _Stream_ s = stream;
	MUTEX_LOCK(s->mutex->lock);
	MUTEX_COND_WAIT(
		vector_len(s->vector) || s->closed,
		s->mutex->sig_produce,
		s->mutex->lock
	);
	StreamElement e = vector_len(s->vector)
		? vector_unshift(s->vector)
		: END_OF_STREAM;
	ASSERT(
		s->closed || e != END_OF_STREAM,
		"END_OF_STREAM asserted before stream is closed"
	)
	pthread_cond_broadcast(s->mutex->sig_consume);
	MUTEX_UNLOCK(s->mutex->lock);
	return e;
}
