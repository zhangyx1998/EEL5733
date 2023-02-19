/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Queue implementations
 */
#include "macros.h"
#include "queue.h"

typedef struct Queue {
	QueueElement *buffer, *delete_p, *insert_p;
	size_t capacity, size;
} * const _Queue_;

#ifdef DEBUG_LIB_QUEUE
#define REPORT_QUEUE_OPERATION(Q, E)		\
	DEBUG_PRINT(							\
		"[%p] range(%zu->%zu) size=%zu",	\
		(void *)E,							\
		(size_t)(Q->delete_p - Q->buffer),	\
		(size_t)(Q->insert_p - Q->buffer),	\
		Q->size								\
	)
#else
#define REPORT_QUEUE_OPERATION(Q, E)
#endif

Queue create_queue(size_t capacity) {
	ASSERT(capacity > 0, "initializing queue with length of 0");
	_Queue_ q = malloc(sizeof(*q));
	q->buffer = malloc(capacity * sizeof(*(q->buffer)));
	q->capacity = capacity;
	q->delete_p = q->buffer;
	q->insert_p = q->buffer;
	q->size = 0;
	REPORT_QUEUE_OPERATION(q, NULL);
	return q;
}

void destroy_queue(Queue queue) {
	_Queue_ q = queue;
	free(q->buffer);
	free(q);
}

size_t queue_len(Queue queue) {
	_Queue_ q = queue;
	return q->size;
}

unsigned short queue_empty(Queue queue) {
	_Queue_ q = queue;
	return q->size == 0;
}

unsigned short queue_full(Queue queue) {
	_Queue_ q = queue;
	return q->size == q->capacity;
}

#define QUEUE_PTR_TO_NEXT(Q, P) { 				\
	if (++(P) >= ((Q)->buffer + (Q)->capacity))	\
		(P) = (Q)->buffer;						\
}

void enqueue(Queue queue, const QueueElement e) {
	_Queue_ q = queue;
	ASSERT(q->size < q->capacity, "write overflow");
	*(q->insert_p) = e;
	QUEUE_PTR_TO_NEXT(q, q->insert_p);
	q->size++;
	REPORT_QUEUE_OPERATION(q, e);
}

QueueElement dequeue(Queue queue) {
	_Queue_ q = queue;
	ASSERT(q->size > 0, "read overflow");
	const QueueElement e = *(q->delete_p);
	QUEUE_PTR_TO_NEXT(q, q->delete_p);
	q->size--;
	REPORT_QUEUE_OPERATION(q, e);
	return e;
}
