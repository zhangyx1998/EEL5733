/**
 * EEL5733 Assignments
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
#ifdef DEBUG_USE_MUTEX
	// Mutex
	pthread_mutex_t *mutex;
	pthread_cond_t *signal;
#else
	// Semaphore
	sem_t *mutex, *empty, *full;
#endif
} model;

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
#ifdef DEBUG_USE_MUTEX
	pthread_mutex_lock(model.mutex);
	while (queue_full(queue))
		TR(pthread_cond_wait)(model.signal, model.mutex);
#else
	sem_wait(model.full);
	sem_wait(model.mutex);
#endif
	// Insert into buffer
	if (chunk != NULL) {
		DEBUG_PRINT("\"%.*s\"", (int)(strlen(chunk) - 1), chunk);
	}
	enqueue(queue, chunk);
	fflush(stdout);
	// Release the lock and send signal to consumer
#ifdef DEBUG_USE_MUTEX
	pthread_mutex_unlock(model.mutex);
	TR(pthread_cond_signal)(model.signal);
#else
	sem_post(model.mutex);
	sem_post(model.empty);
#endif
	// Return 0 indicates success
	return 0;
}

ssize_t thread_getline(char **buf, size_t *len) {
	if (*len < 128) *buf = realloc(*buf, *len = 128);
	// Acquire lock
#ifdef DEBUG_USE_MUTEX
	pthread_mutex_lock(model.mutex);
	while (queue_empty(queue))
		TR(pthread_cond_wait)(model.signal, model.mutex);
#else
	sem_wait(model.empty);
	sem_wait(model.mutex);
#endif
	// Pop from buffer
	dequeue(queue, *buf);
	size_t l = strlen(*buf);
	DEBUG_PRINT("(%zu)\n%s", l, *buf);
	// Release the lock and send signal to producer
#ifdef DEBUG_USE_MUTEX
	pthread_mutex_unlock(model.mutex);
	TR(pthread_cond_signal)(model.signal);
#else
	sem_post(model.mutex);
	sem_post(model.full);
#endif
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
	// Initialize lock
#ifdef DEBUG_USE_MUTEX
	DEBUG_PRINT("#### USING MUTEX LOCK ####");
	void *s = mmap(
		NULL, sizeof(pthread_mutex_t) + sizeof(pthread_cond_t),
		PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_SHARED,
		-1, 0
	);
	model.mutex = (pthread_mutex_t *)s;
	model.signal = (pthread_cond_t *)(model.mutex + 1);
	pthread_mutex_init(model.mutex, NULL);
	pthread_cond_init(model.signal, NULL);
#elif defined __MACH__ // MacOS
	DEBUG_PRINT("#### USING NAMED SEMAPHORES ####");
	// Support for unnamed semaphore on MacOS was broken,
	// use named semaphore instead.
	#define SEM_PREFIX "/location_updater.model."
	model.mutex = sem_open(SEM_PREFIX"mutex", O_CREAT, 0x777, 1);
	model.empty = sem_open(SEM_PREFIX"empty", O_CREAT, 0x777, 0);
	model.full  = sem_open(SEM_PREFIX"full" , O_CREAT, 0x777, queue_size);
#else // POSIX Generic
	DEBUG_PRINT("#### USING UNNAMED SEMAPHORES ####");
	// Initialize mmap region for semaphores
	sem_t *s = mmap(
		NULL, 3 * sizeof(sem_t),
		PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_SHARED,
		-1, 0
	);
	// Initialize unnamed semaphore
	sem_init((model.mutex = &s[0]), 1, 1);
	sem_init((model.empty = &s[1]), 1, 0);
	sem_init((model.full = &s[2]), 1, queue_size);
#endif
	// Initialize queue
	queue = create_queue(queue_size, 128);
	// Create child processes
	int pid[2];
	if ((pid[0] = fork()) == 0) {
		// Child process 1
		consumer(consumer_entry);
		exit(0);
	} else if ((pid[1] = fork()) == 0) {
		// Child process 2
		producer(producer_entry);
		exit(0);
	} else {
		// Main process
		// Check for child status
		int status;
		for (int i = 0; i < 2; i ++) {
			waitpid(pid[i], &status, 0);
			ASSERT(
				status == 0,
				"Process %d exited with code %d", pid[i], status
			);
		}
		destroy_queue(queue);
		DEBUG_PRINT("MAIN PROCESS EXIT");
	}
}
