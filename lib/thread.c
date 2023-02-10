/**
 * EEL5733 Assignment 2
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief p_thread library wrapper functions 
 */
#include "pthread.h"
#include "ctype.h"
#include "stdio.h"
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

int thread_putchar(const char c) {
	pthread_mutex_lock(&model.lock);
	while (queue_full(queue))
		pthread_cond_wait(&model.producer, &model.lock);
	// Insert into buffer
	enqueue(queue, (QueueElement)c);
	// Debug log
	DEBUG_PRINT(
		isprint(c)
			? "Thread::thread_putchar '%c' <%zu>\n"
			: "Thread::thread_putchar (%d) <%zu>\n",
		c, queue_len(queue)
	);
	fflush(stdout);
	// Release the lock and send signal to consumer
	pthread_mutex_unlock(&model.lock);
	pthread_cond_signal(&model.consumer);
	// Return count of chars put into the buffer
	return 1;
}

int thread_puts(const char *str) {
	int l = 0;
	DEBUG_PRINT("Thread::puts \"%s\"\n", str);
	while ((*str) != 0 && (*str) != EOF) {
		int result = thread_putchar(*(str++));
		if (result < 0) return result;
		else l += result;
	}
	int result = thread_putchar('\n');
	if (result < 0) return result;
	return l;
}

const char thread_getchar() {
	pthread_mutex_lock(&model.lock);
	while (queue_empty(queue))
		pthread_cond_wait(&model.consumer, &model.lock);
	// Pop from buffer
	const char c = dequeue(queue);
	// Debug log
	DEBUG_PRINT(
		isprint(c)
			? "Thread::thread_getchar '%c' <%zu>\n"
			: "Thread::thread_getchar (%d) <%zu>\n",
		c, queue_len(queue)
	);
	// Release the lock and send signal to producer
	pthread_mutex_unlock(&model.lock);
	pthread_cond_signal(&model.producer);
	// Return popped value
	return c;
}

#define GETLINE_CHUNK_SIZE 128
ssize_t thread_getline(char **buf, size_t *len) {
	static char c = 0;
	if (c == EOF) return EOF;
	size_t l = 0;
	while (1) {
		if (*len <= l) {
			// Calculate enlarged buffer size
			*len += GETLINE_CHUNK_SIZE - (*len) % GETLINE_CHUNK_SIZE;
			// Reallocate buffer
			*buf = realloc(*buf, *len);
		}
		c = thread_getchar();
		if (c == '\0' || c == EOF) break;
		(*buf)[l++] = c;
		if (c == '\n') break;
	}
	(*buf)[l] = 0;
	DEBUG_PRINT("Thread::getline >> \"%s\"\n", *buf);
	return l;
}

void *producer(void *entry) {
	// Initialize IO descriptor
	const struct IO io = {
		.getline=&stdin_getline,
		.puts=&thread_puts
	};
	// Enter thread main function
	DEBUG_PRINT("Thread::producer >> LAUNCH [%p]\n", entry);
	(*(ThreadEntry)entry)(&io);
	// Send EOF signal
	thread_putchar(EOF);
	// Report exit in debug mode
	DEBUG_PRINT("Thread::producer >> EXIT\n");
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
	DEBUG_PRINT("Thread::consumer >> LAUNCH [%p]\n", entry);
	(*(ThreadEntry)entry)(&io);
	// Report exit in debug mode
	DEBUG_PRINT("Thread::consumer >> EXIT\n");
	// Clean up context before exit
	fflush(stdout);
	fflush(stderr);
	return NULL;
}

void launch(ThreadEntry producer_entry, ThreadEntry consumer_entry, size_t queue_size) {
	DEBUG_PRINT("Thread::launch(%p, %p)\n", &producer_entry, &consumer_entry);
	// Thread identifiers
	pthread_t tid[2];
	// Initialize queue
	queue = create_queue(queue_size);
	// Create the threads; may be any number, in general
	if (pthread_create(&tid[0], NULL, producer, producer_entry))
		ERR_EXIT("Unable to create producer\n");
	if (pthread_create(&tid[1], NULL, consumer, consumer_entry))
		ERR_EXIT("Unable to create consumer\n");
	// Wait for created thread to exit
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	DEBUG_PRINT("Thread::launch >> MAIN THREAD EXIT\n");
	// Destroy queue
	free_queue(queue);
}