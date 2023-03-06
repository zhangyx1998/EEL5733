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
	if (*len < 128) *buf = realloc(*buf, *len = 128);
	// Acquire lock
	ASSERT(
		pthread_mutex_lock(&model.lock) == 0,
		"unable to acquire lock"
	);
	while (queue_empty(queue))
		pthread_cond_wait(&model.consumer, &model.lock);
	// Pop from buffer
	dequeue(queue, *buf);
	size_t l = strlen(*buf);
	// Release the lock and send signal to producer
	pthread_mutex_unlock(&model.lock);
	pthread_cond_signal(&model.producer);
	// Return result of getline
	return l > 0 ? l : EOF;
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
	// Send empty string as EOF
	thread_puts("");
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
	// pthread_t tid[2];
	int pid[2];
	// Initialize queue
	Queue queue = create_queue(queue_size, 128);
	// Create the threads; may be any number, in general
	if ((pid[0] = fork()) == 0)
		// Child process 1
		producer(producer_entry);
	else if ((pid[1] = fork()) == 0)
		// Child process 2
		consumer(consumer_entry);
	else {
		// Main process
		waitpid(pid[0], NULL, 0);
		waitpid(pid[1], NULL, 0);
		DEBUG_PRINT("MAIN THREAD EXIT");
	}
	// Destroy queue
	destroy_queue(queue);
}
