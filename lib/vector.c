/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Vector implementations
 */
#include "string.h"
#include "lock.h"
#include "macros.h"
#include "vector.h"

typedef struct {
	size_t capacity, start, end;
	VectorElement *buffer;
} * const _Vector_;

#define S(L) ((L) * sizeof(VectorElement))

#define OVERFLOW_MSG(I, L, R) \
	"Index [%zu] out of range %zu-%zu", \
	(size_t)(I), (size_t)(L), (size_t)(R)

#ifdef DEBUG_LIB_VECTOR
#define PRINT_VECTOR(V)										\
	DEBUG_PRINT(											\
		"[%p] length %zu, range %zu-%zu (capacity %zu)",	\
		(void *)V, vector_len((Vector)V), (V)->start, (V)->end, (V)->capacity			\
	)
#else
#define PRINT_VECTOR(V)
#endif

// Internal API

void _vector_fit_(_Vector_ v, size_t size) {
	if (size > v->capacity) {
		v->capacity += VECTOR_CHUNK_SIZE - (v->capacity % VECTOR_CHUNK_SIZE);
		v->buffer = realloc(v->buffer, S(v->capacity));
	} else if (v->capacity - size > VECTOR_CHUNK_SIZE) {
		v->capacity = size + VECTOR_CHUNK_SIZE - size % VECTOR_CHUNK_SIZE;
		v->buffer = realloc(v->buffer, S(v->capacity));
	}
}

// Public API

Vector vector() {
	_Vector_ v = malloc(sizeof(*v));
	v->capacity = 0;
	v->start = v->end = 0;
	v->buffer = NULL;
#ifdef DEBUG_LIB_VECTOR
	DEBUG_PRINT(
		"initializing %p (capacity %zu, length %zu)",
		(void *)v, v->capacity, v->end
	);
#endif
	return (Vector)v;
}

void _vector(Vector vector) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	free(v->buffer);
	free(v);
}

size_t vector_len(Vector vector) {
	_Vector_ v = vector;
	ASSERT(
		v->end >= v->start,
		"vector end index (%zu) lower than start index (%zu)",
		v->end, v->start
	);
	return v->end - v->start;
}

VectorElement vector_get(Vector vector, size_t i) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->start + i < v->end, OVERFLOW_MSG(i, v->start, v->end));
	return (v->buffer)[v->start + i];
}

void vector_set(Vector vector, size_t i, VectorElement e) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->start + i < v->end, OVERFLOW_MSG(i, v->start, v->end));
	(v->buffer)[v->start + i] = e;
}

void vector_push(Vector vector, VectorElement e) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	_vector_fit_(v, v->end + 1);
	(v->buffer)[v->end++] = e;
}

VectorElement vector_pop(Vector vector) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->end > v->start, OVERFLOW_MSG(1, v->start, v->end));
	VectorElement e = (v->buffer)[--(v->end)];
	_vector_fit_(v, v->end);
	return e;
}

void vector_shift(Vector vector, VectorElement e) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	// Expand space on the left if necessary
	if (v->start == 0) {
		// expand vector left side space
		_vector_fit_(v, v->end + VECTOR_CHUNK_SIZE);
		memmove(
			v->buffer + VECTOR_CHUNK_SIZE,
			v->buffer,
			S(v->end)
		);
		// Move pointers to new space
		v->start += VECTOR_CHUNK_SIZE;
		v->end += VECTOR_CHUNK_SIZE;
	}
	(v->buffer)[--(v->start)] = e;
}

VectorElement vector_unshift(Vector vector) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->end > v->start, OVERFLOW_MSG(1, v->start, v->end));
	VectorElement e = (v->buffer)[v->start++];
	// Shrink space on the left if necessary
	if (v->start >= VECTOR_CHUNK_SIZE) {
		size_t
			start = v->start % VECTOR_CHUNK_SIZE,
			offset = v->start - start;
		// expand vector left side space
		memmove(
			v->buffer + start,
			v->buffer + v->start,
			S(vector_len(vector))
		);
		// Move pointers to new space
		v->end -= offset;
		v->start = start;
		_vector_fit_(v, v->end);
	}
	return e;
}

void vector_insert(Vector vector, size_t i, VectorElement e) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->start + i <= v->end, OVERFLOW_MSG(i, v->start, v->end));
	_vector_fit_(v, v->end + 1);
	if (v->end > i)
		memmove(
			v->buffer + i + 1,
			v->buffer + i,
			S(vector_len(vector) - i)
		);
	v->end++;
	v->buffer[v->start + i] = e;
}

VectorElement vector_delete(Vector vector, size_t i) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->start + i < v->end, OVERFLOW_MSG(i, v->start, v->end));
	VectorElement e = v->buffer[v->start + i];
	if (v->end > v->start + i + 1)
		memmove(
			v->buffer + v->start + i,
			v->buffer + v->start + i + 1,
			S(vector_len(vector) - 1 - i)
		);
	v->end--;
	return e;
}
