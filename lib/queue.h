/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Queue Header
 */
#ifndef QUEUE_H
#define QUEUE_H

#include "stdlib.h"

typedef void * Queue;
typedef char * QueueElement;

// Dynamically create queue with given capacity
Queue create_queue(size_t);
void destroy_queue(Queue);

size_t queue_len(Queue);
unsigned short queue_empty(Queue);
unsigned short queue_full(Queue);

void enqueue(Queue  const, const QueueElement);
QueueElement dequeue(Queue);

#endif
