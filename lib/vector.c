/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Vector implementations
 */
#include "string.h"
#include "vector.h"
#include "macros.h"

typedef struct {
	size_t capacity, length;
	VectorElement *buffer;
} * const _Vector_;

#define S(L) ((L) * sizeof(VectorElement))

#define OVERFLOW_MSG(I, L) \
	"Index [%zu] out of range (%zu)", \
	(size_t)(I), (size_t)(L)

#ifdef DEBUG_LIB_VECTOR
#define PRINT_VECTOR(V)						\
	DEBUG_PRINT(							\
		"[%p] length %zu (capacity %zu)",	\
		(void *)v, v->length, v->capacity	\
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
	v->length = 0;
	v->buffer = NULL;
#ifdef DEBUG_LIB_VECTOR
	DEBUG_PRINT(
		"initializing %p (capacity %zu, length %zu)",
		(void *)v, v->capacity, v->length
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
	return v->length;
}

VectorElement vector_get(Vector vector, size_t i) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->length > i, OVERFLOW_MSG(i, v->length));
	return (v->buffer)[i];
}

void vector_set(Vector vector, size_t i, VectorElement e) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->length > i, OVERFLOW_MSG(i, v->length));
	(v->buffer)[i] = e;
}

void vector_push(Vector vector, VectorElement e) {
	_Vector_ v = vector;
	_vector_fit_(v, v->length + 1);
	(v->buffer)[v->length++] = e;
	PRINT_VECTOR(v);
}

VectorElement vector_pop(Vector vector) {
	_Vector_ v = vector;
	ASSERT(v->length >= 1, OVERFLOW_MSG(1, v->length));
	VectorElement e = (v->buffer)[--(v->length)];
	_vector_fit_(v, v->length);
	PRINT_VECTOR(v);
	return e;
}

void vector_shift(Vector vector, VectorElement e) {
	_Vector_ v = vector;
	_vector_fit_(v, ++(v->length));
	PRINT_VECTOR(v);
	memmove(
		v->buffer + 1,
		v->buffer,
		S(v->length)
	);
	(v->buffer)[0] = e;
	(v->length)++;
}

VectorElement vector_unshift(Vector vector) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->length >= 1, OVERFLOW_MSG(1, v->length));
	VectorElement e = (v->buffer)[0];
	memmove(
		v->buffer,
		v->buffer + 1,
		S(--(v->length))
	);
	_vector_fit_(v, v->length);
	return e;
}

void vector_insert(Vector vector, size_t i, VectorElement e) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->length >= i, OVERFLOW_MSG(i, v->length));
	_vector_fit_(v, v->length + 1);
	if (v->length > i)
		memmove(
			v->buffer + i + 1,
			v->buffer + i,
			S(v->length - i)
		);
	v->length++;
	v->buffer[i] = e;
}

VectorElement vector_delete(Vector vector, size_t i) {
	_Vector_ v = vector;
	PRINT_VECTOR(v);
	ASSERT(v->length > i, OVERFLOW_MSG(i, v->length));
	VectorElement e = v->buffer[i];
	if (v->length - 1 > i)
		memmove(
			v->buffer + i,
			v->buffer + i + 1,
			S(v->length - 1 - i)
		);
	v->length--;
	return e;
}
