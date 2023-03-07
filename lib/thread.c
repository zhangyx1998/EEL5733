/**
 * EEL5733 Assignment 2
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief p_thread library wrapper functions 
 */
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "macros.h"
#include "thread.h"
#include "queue.h"
#include "semaphore.h"

// Shared ring-buffer
Queue queue;
struct LockModel {
	sem_t mutex, empty, full;
} * model;

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
		chunk[len + 0] = len ? '\n' : '\0';
		chunk[len + 1] = '\0';
	}
	// Acquire lock
	sem_wait(&model->full);
	sem_wait(&model->mutex);
	// Insert into buffer
	if (chunk != NULL) {
		DEBUG_PRINT("\"%.*s\"", (int)(strlen(chunk) - 1), chunk);
	}
	enqueue(queue, chunk);
	fflush(stdout);
	// Release the lock and send signal to consumer
	sem_post(&model->mutex);
	sem_post(&model->empty);
	// Return 0 indicates success
	return 0;
}

ssize_t thread_getline(char **buf, size_t *len) {
	if (*len < 128) *buf = realloc(*buf, *len = 128);
	// Acquire lock
	sem_wait(&model->empty);
	sem_wait(&model->mutex);
	// Pop from buffer
	dequeue(queue, *buf);
	size_t l = strlen(*buf);
	DEBUG_PRINT("(%zu)\n%s", l, *buf);
	// Release the lock and send signal to producer
	sem_post(&model->mutex);
	sem_post(&model->full);
	// Return result of getline
	return l > 0 ? l : EOF;
}

void producer(ThreadEntry entry) {
	// Initialize IO descriptor
	const struct IO io = {
		.getline=&stdin_getline,
		.puts=&thread_puts
	};
	// Enter thread main function
	DEBUG_PRINT("LAUNCH [%p]", entry);
	entry(&io);
	// Send empty string as EOF
	thread_puts("");
	// Report exit in debug mode
	DEBUG_PRINT("PRODUCER EXIT");
	// Clean up context before exit
	fflush(stdout);
	fflush(stderr);
}

void consumer(ThreadEntry entry) {
	// Initialize IO descriptor
	const struct IO io = {
		.getline=&thread_getline,
		.puts=&puts
	};
	// Enter thread main function
	DEBUG_PRINT("LAUNCH [%p]", entry);
	entry(&io);
	// Report exit in debug mode
	DEBUG_PRINT("CONSUMER EXIT");
	// Clean up context before exit
	fflush(stdout);
	fflush(stderr);
}

void launch(ThreadEntry producer_entry, ThreadEntry consumer_entry, size_t queue_size) {
	DEBUG_PRINT("[%p], [%p]", &producer_entry, &consumer_entry);
	// Thread identifiers
	int pid;
	// Initialize lock
	model = mmap(NULL, sizeof(*model), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	sem_init(&model->mutex, 1, 1);
	sem_init(&model->empty, 1, 0);
	sem_init(&model->full, 1, (unsigned int)queue_size);
	// Initialize queue
	queue = create_queue(queue_size, 128);
	// Create the threads; may be any number, in general
	if ((pid = fork()) == 0) {
		// Child process 1
		consumer(consumer_entry);
		exit(0);
	} else {
		// Main process
		producer(producer_entry);
		// Check for child status
		int status;
		waitpid(pid, &status, 0);
		ASSERT(
			status == 0,
			"Process %d exited with code %d", pid, status
		);
		destroy_queue(queue);
		DEBUG_PRINT("MAIN PROCESS EXIT");
	}
}
