/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Stream Header
 */
#ifndef STREAM_H
#define STREAM_H

#include "stdio.h"
#include "queue.h"

typedef void * Stream;
typedef QueueElement StreamElement;

#define END_OF_STREAM ((StreamElement)EOF)

Stream stream();
void _stream(Stream);

void stream_write(Stream, StreamElement);
StreamElement stream_read(Stream);

#endif
