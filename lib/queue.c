/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Queue implementations
 */
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"
#include "queue.h"

typedef struct Queue {
	QueueElement buffer, delete_p, insert_p;
	size_t capacity, length, element_size;
} * _Queue_;

#ifdef DEBUG_LIB_QUEUE
#define REPORT_QUEUE_OPERATION(Q, E)		\
	DEBUG_PRINT(							\
		"[%p] range(%zu->%zu) size=%zu",	\
		(void *)E,							\
		(size_t)((Q->delete_p - Q->buffer) / Q->element_size),	\
		(size_t)((Q->insert_p - Q->buffer) / Q->element_size),	\
		Q->length							\
	)
#else
#define REPORT_QUEUE_OPERATION(Q, E)
#endif

Queue create_queue(size_t capacity, size_t element_size) {
	ASSERT(capacity > 0, "initializing queue with length of 0");
	ASSERT(element_size > 0, "initializing queue with element size of 0");
	_Queue_ q;
	size_t total = sizeof(*q) + capacity * element_size;
	void *space = mmap(NULL, total, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
#ifdef DEBUG_LIB_QUEUE
	DEBUG_PRINT(
		"Initializing queue with capacity %zu, el size %zu, total %zu",
		capacity, element_size, total
	);
	DEBUG_PRINT("MMAP -> %p", space);
#endif
	q = space;
	q->buffer = space + sizeof(*q);
	q->element_size = element_size;
	q->delete_p = q->buffer;
	q->insert_p = q->buffer;
	q->capacity = capacity;
	q->length = 0;
	REPORT_QUEUE_OPERATION(q, q->buffer);
	return q;
}

void destroy_queue(Queue queue) {
	_Queue_ q = queue;
	munmap(q, sizeof(*q) + q->capacity * q->element_size);
}

size_t queue_len(Queue queue) {
	_Queue_ q = queue;
	return q->length;
}

unsigned short queue_empty(Queue queue) {
	_Queue_ q = queue;
	return q->length == 0;
}

unsigned short queue_full(Queue queue) {
	_Queue_ q = queue;
	return q->length == q->capacity;
}

#define QUEUE_PTR_TO_NEXT(Q, P) { 							\
	if (													\
		((P) += (Q)->element_size) >=						\
		((Q)->buffer + (Q)->capacity * (Q)->element_size)	\
	)														\
		(P) = (Q)->buffer;									\
}

void enqueue(Queue queue, const QueueElement e) {
	_Queue_ q = queue;
	ASSERT(q->length < q->capacity, "write overflow");
	memcpy(q->insert_p, e, q->element_size);
	QUEUE_PTR_TO_NEXT(q, q->insert_p);
	q->length++;
	REPORT_QUEUE_OPERATION(q, e);
}

QueueElement dequeue(Queue queue, QueueElement e) {
	_Queue_ q = queue;
	ASSERT(q->length > 0, "read overflow");
	memcpy(e, q->delete_p, q->element_size);
	QUEUE_PTR_TO_NEXT(q, q->delete_p);
	q->length--;
	REPORT_QUEUE_OPERATION(q, e);
	return e;
}
