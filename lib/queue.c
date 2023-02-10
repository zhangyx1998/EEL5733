#include "macros.h"
#include "queue.h"

struct Queue {
	QueueElement **buffer, **start, **end;
	size_t capacity;
	unsigned short empty;
};

typedef struct Queue Queue;

void *create_queue(size_t capacity) {
	if (capacity == 0)
		ERR_EXIT("Queue::create: initializing queue with length of 0\n");
	Queue * const q = malloc(sizeof(Queue));
	q->buffer = malloc(capacity * sizeof(*q->buffer));
	q->capacity = capacity;
	q->start = q->buffer;
	q->end = q->buffer;
	q->empty = 1;
	return (void *)q;
}

void free_queue(void * const queue) {
	Queue * const q = queue;
	free(q->buffer);
	free(q);
}

size_t queue_len(void * const queue) {
	Queue * const q = queue;
	if (q->empty)
		return 0;
	else if (q->start < q->end)
		return q->end - q->start;
	else
		return q->capacity - (q->start - q->end);
}

unsigned short queue_empty(void * const queue) {
	Queue * const q = queue;
	return q->empty;
}

unsigned short queue_full(void * const queue) {
	Queue * const q = queue;
	return (q->start == q->end) && !q->empty;
}

#define MOVE_NEXT(Q, P) { 					\
	if (++P >= (Q)->buffer + (Q)->capacity)	\
		P = (Q)->buffer;					\
}

void enqueue(void * const queue, const QueueElement c) {
	Queue * const q = queue;
	*q->end = c;
	MOVE_NEXT(q, q->end);
	q->empty = 0;
}

QueueElement dequeue(void * const queue) {
	Queue * const q = queue;
	const QueueElement c = *q->start;
	MOVE_NEXT(q, q->start);
	if (q->start == q->end)
		q->empty = 1;
	return c;
}
