/**
 * EEL5733 Assignment 2
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Queue header
 */
#ifndef QUEUE_H
#define QUEUE_H

#include "stdlib.h"

typedef void * QueueElement;

// Dynamically create queue with given capacity
void *create_queue(size_t);
void free_queue(void * const);

size_t queue_len(void * const);
unsigned short queue_empty(void * const);
unsigned short queue_full(void * const);

void enqueue(void * const, const QueueElement);
QueueElement dequeue(void * const);

#endif
