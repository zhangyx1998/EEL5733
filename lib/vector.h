/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Vector Header
 */
#ifndef VECTOR_H
#define VECTOR_H

#include "stdlib.h"

#define VECTOR_CHUNK_SIZE 128

typedef void * Vector;
typedef void * VectorElement;

Vector vector();
void _vector(Vector);

size_t vector_len(Vector);

VectorElement vector_get(Vector, size_t);
void vector_set(Vector, size_t, VectorElement);

void vector_push(Vector, VectorElement);
VectorElement vector_pop(Vector);

void vector_shift(Vector, VectorElement);
VectorElement vector_unshift(Vector);

void vector_insert(Vector, size_t, VectorElement);
VectorElement vector_delete(Vector, size_t);

#endif
