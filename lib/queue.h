/**
 * EEL5733 Assignment 2
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Queue header
 */
#ifndef QUEUE_H
#define QUEUE_H

#include "stdlib.h"

struct Queue {
	char *buffer;
	size_t size;
	char *start, *end;
	unsigned short flag_empty;
};

typedef struct Queue Queue;

Queue *create_queue(size_t size);
void free_queue(Queue * const);

size_t queue_len(Queue * const);

void enqueue(Queue * const, const char);
char dequeue(Queue * const);

#endif
