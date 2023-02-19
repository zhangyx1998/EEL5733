/**
 * EEL5733 Assignment 3
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Standalone Test Program For "Stream"
 */

#include "stdio.h"
#include "pthread.h"
/* ======= project library ====== */
#include "stream.h"
#include "macros.h"

Stream s;
int *list = NULL;

void *producer(size_t test_size) {
	list = realloc(list, test_size);
	for (size_t i = 0; i < test_size; i++) {
		list[i] = (i + 1) & 1 ? i + 1 : -i;
		stream_write(s, (StreamElement)&list[i]);
	}
	// Close stream
	stream_write(s, END_OF_STREAM);
	return NULL;
}

void *worker(void *p) {
	int *i;
	size_t count = 0;
	while ((i = (int *)stream_read(s)) != (int *)END_OF_STREAM) {
		ASSERT((*i) != 0, "%p has been accessed", (void *)i);
		(*i) = 0;
		count ++;
	}
	DEBUG_PRINT("completed %zd tasks", count);
	return NULL;
}

int main(int argc, const char *argv[]) {
	size_t test_size;
	scanf("%zd", &test_size);
	// Parse optional command line argument
	const size_t
		n_threads = (size_t)(argc >= 2 ? atoi(argv[1]) : 10),
		buf_size  = (size_t)(argc >= 3 ? atoi(argv[2]) : 16);
	// Initialize stream
	s = stream(buf_size);
	// Launch execution workers
	pthread_t *tid = malloc(n_threads * sizeof(*tid));
	for (size_t i = 0; i < n_threads; i++)
		pthread_create(tid + i, NULL, worker, NULL);
	// Instruction producer loop
	producer(test_size);
	// Wait for all thread to end
	for (size_t i = 0; i < n_threads; i++)
		pthread_join(tid[i], NULL);
	// Check correctness
	size_t checked_size = 0;
	for (size_t i = 0; i < test_size; i++) {
		int v = list[i];
		if (v != 0) EPRINT(
			"[%p] = %d was not accessed",
			(void *)&(list[i]), v
		);
		else checked_size++;
	}
	printf("%zu\n", checked_size);
	return 0;
}
