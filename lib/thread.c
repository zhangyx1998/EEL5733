/**
 * EEL5733 Assignment 2
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief p_thread library wrapper functions 
 */
#include "pthread.h"
#include "ctype.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "macros.h"
#include "thread.h"
#include "queue.h"

// Shared ring-buffer
void *queue;
struct LockModel {
	pthread_mutex_t lock;
	pthread_cond_t consumer, producer;
} model = {
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_COND_INITIALIZER,
	PTHREAD_COND_INITIALIZER
};

int thread_puts(const char *str) {
	char *chunk = NULL;
	// Copy and add \n to end of string
	if (str != NULL) {
		const size_t len = strlen(str);
		chunk = memcpy(
			malloc((len + 2) * sizeof(char)),
			str,
			(len + 1) * sizeof(char)
		);
		chunk[len + 0] = '\n';
		chunk[len + 1] = '\0';
	}
	// Acquire lock
	ASSERT(
		pthread_mutex_lock(&model.lock) == 0,
		"unable to acquire lock"
	);
	while (queue_full(queue))
		pthread_cond_wait(&model.producer, &model.lock);
	// Insert into buffer
	if (chunk != NULL) {
		DEBUG_PRINT("\"%.*s\"", (int)(strlen(chunk) - 1), chunk);
	}
	enqueue(queue, chunk);
	fflush(stdout);
	// Release the lock and send signal to consumer
	pthread_mutex_unlock(&model.lock);
	pthread_cond_signal(&model.consumer);
	// Return 0 indicates success
	return 0;
}

ssize_t thread_getline(char **buf, size_t *len) {
	// Flag to exit for EOF on next call of this function
	static int flag_EOF = 0;
	if (flag_EOF) return EOF;
	// Static chunk pointer to continue scanning unfinished chunk
	static const char *chunk = NULL, *trim = NULL;
	// Total size of string stored into buffer
	size_t total = 0;
	// Keep capturing until line is complete
	while (1) {
		if (chunk != NULL) {
			// Scan previous chunk and find next line feed
			size_t step = 0;
			while (trim[step] != '\n' && trim[step] != '\0') step++;
			if (trim[step] == '\n') step++;
			// Fit new content into buffer
			if (total + step >= *len) {
				*len = total + step + 1;
				*buf = realloc(*buf, *len);
			}
			memcpy(*buf + total, trim, step);
			total += step;
			(*buf)[total] = 0;
			// Free chunk if finished
			trim += step;
			if (*trim == 0) {
				free((void *)chunk);
				chunk = NULL;
			}
			// Return if buffer ends with line feed
			if ((*buf)[total - 1] == '\n') {
				DEBUG_PRINT("\"%.*s\"", (int)(total - 1), *buf);
				return total;
			}
		} else {
			// Get next string chunk
			// Acquire lock
			ASSERT(
				pthread_mutex_lock(&model.lock) == 0,
				"unable to acquire lock"
			);
			while (queue_empty(queue))
				pthread_cond_wait(&model.consumer, &model.lock);
			// Pop from buffer
			const char *next_chunk = dequeue(queue);
			// Release the lock and send signal to producer
			pthread_mutex_unlock(&model.lock);
			pthread_cond_signal(&model.producer);
			// Check for EOF
			if (next_chunk == NULL) {
				flag_EOF = 1;
				return total > 0 ? total : EOF;
			}
			// Assign next chunk to the static pointer
			ASSERT(chunk == NULL, "current chunk not set to NULL");
			chunk = next_chunk;
			trim = chunk;
		}
	}
	// Trap: control should NEVER reach here
	ERR_EXIT("control ran into unreachable point");
}

void *producer(void *entry) {
	// Initialize IO descriptor
	const struct IO io = {
		.getline=&stdin_getline,
		.puts=&thread_puts
	};
	// Enter thread main function
	DEBUG_PRINT("LAUNCH [%p]", entry);
	(*(ThreadEntry)entry)(&io);
	// Send NULL as EOF signal
	thread_puts(NULL);
	// Report exit in debug mode
	DEBUG_PRINT("EXIT");
	// Clean up context before exit
	fflush(stdout);
	fflush(stderr);
	return NULL;
}

void *consumer(void *entry) {
	// Initialize IO descriptor
	const struct IO io = {
		.getline=&thread_getline,
		.puts=&puts
	};
	// Enter thread main function
	DEBUG_PRINT("LAUNCH [%p]", entry);
	(*(ThreadEntry)entry)(&io);
	// Report exit in debug mode
	DEBUG_PRINT("EXIT");
	// Clean up context before exit
	fflush(stdout);
	fflush(stderr);
	return NULL;
}

void launch(ThreadEntry producer_entry, ThreadEntry consumer_entry, size_t queue_size) {
	DEBUG_PRINT("[%p], [%p]", &producer_entry, &consumer_entry);
	// Thread identifiers
	pthread_t tid[2];
	// Initialize queue
	queue = create_queue(queue_size);
	// Create the threads; may be any number, in general
	if (pthread_create(&tid[0], NULL, producer, producer_entry))
		ERR_EXIT("unable to create producer");
	if (pthread_create(&tid[1], NULL, consumer, consumer_entry))
		ERR_EXIT("unable to create consumer");
	// Wait for created thread to exit
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	DEBUG_PRINT("MAIN THREAD EXIT");
	// Destroy queue
	free_queue(queue);
}
