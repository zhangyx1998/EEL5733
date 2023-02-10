#include "macros.h"
#include "queue.h"

Queue *create_queue(size_t size) {
	if (size == 0)
		ERR_EXIT("Queue::create: initializing queue with length of 0\n");
	Queue * const q = malloc(sizeof(Queue));
	q->buffer = malloc(size * sizeof(char));
	q->size = size;
	q->start = q->buffer;
	q->end = q->buffer;
	q->flag_empty = 1;
	return q;
}

void free_queue(Queue * const q) {
	free(q->buffer);
	free(q);
}

size_t queue_len(Queue * const q) {
	if (q->flag_empty)
		return 0;
	else if (q->start < q->end)
		return q->end - q->start;
	else
		return q->size - (q->start - q->end);
}

#define MOVE_NEXT(Q, P) 					\
	if (++P >= (Q)->buffer + (Q)->size)	\
		P = (Q)->buffer;

void enqueue(Queue * const q, const char c) {
	*q->end = c;
	MOVE_NEXT(q, q->end);
	q->flag_empty = 0;
}

char dequeue(Queue * const q) {
	const char c = *q->start;
	MOVE_NEXT(q, q->start);
	if (q->start == q->end)
		q->flag_empty = 1;
	return c;
}
