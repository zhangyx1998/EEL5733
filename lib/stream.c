/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Stream implementations
 */
#include "pthread.h"
#include "lock.h"
#include "queue.h"
#include "stream.h"
#include "macros.h"

typedef struct {
	Queue queue;
	int closed;
	PC_Lock mutex;
} * _Stream_;

Stream stream(size_t queue_size) {
	_Stream_ s = malloc(sizeof(*s));
	s->queue = create_queue(queue_size);
	s->closed = 0;
	s->mutex = create_PC_lock();
	return s;
}

void _stream(Stream stream) {
	const _Stream_ s = stream;
	destroy_queue(s->queue);
	destroy_PC_lock(s->mutex);
	free((void *)s);
}

void stream_write(Stream stream, StreamElement e) {
	const _Stream_ s = stream;
	MUTEX_LOCK(s->mutex->lock);
	// Check if writing EOS to stream
	if (e == END_OF_STREAM) {
		MUTEX_COND_WAIT(
			queue_empty(s->queue),
			s->mutex->sig_consume,
			s->mutex->lock
		);
		s->closed = 1;
		DEBUG_PRINT("stream [%p] closed", (void *)s);
	} else {
		MUTEX_COND_WAIT(
			!queue_full(s->queue),
			s->mutex->sig_consume,
			s->mutex->lock
		);
		enqueue(s->queue, e);
	}
	pthread_cond_broadcast(s->mutex->sig_produce);
	MUTEX_UNLOCK(s->mutex->lock);
}

StreamElement stream_read(Stream stream) {
	const _Stream_ s = stream;
	MUTEX_LOCK(s->mutex->lock);
	MUTEX_COND_WAIT(
		!queue_empty(s->queue) || s->closed,
		s->mutex->sig_produce,
		s->mutex->lock
	);
	StreamElement e = queue_empty(s->queue)
		? END_OF_STREAM
		: dequeue(s->queue);
	ASSERT(
		s->closed || e != END_OF_STREAM,
		"END_OF_STREAM asserted before stream is closed"
	)
	pthread_cond_broadcast(s->mutex->sig_consume);
	MUTEX_UNLOCK(s->mutex->lock);
	return e;
}
