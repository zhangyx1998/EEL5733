#include "macros.h"
#include "util.h"
#include "unistd.h"
#include "libgen.h"
#include "sys/wait.h"

#define EXECUTABLE_1 "email_filter.bin"
#define EXECUTABLE_2 "calendar_filter.bin"

#define RD 0
#define WR 1

#define PIPE_XX(FD_PAIR)						\
	close(FD_PAIR[RD]);							\
	close(FD_PAIR[WR]);

#define PIPE_RD(FD_PAIR)						\
	dup2(FD_PAIR[RD], STDIN_FILENO);			\
	PIPE_XX(FD_PAIR);

#define PIPE_WR(FD_PAIR)						\
	dup2(FD_PAIR[WR], STDOUT_FILENO);			\
	PIPE_XX(FD_PAIR);

#define EXEC(EXEC_NAME)							\
	char path[256];								\
	/* Execute child program */					\
	sprintf(path, "%s/%s", dir, EXEC_NAME);		\
	execl(path, path, NULL);					\
	/* Report error if reaches here */			\
	EPRINT("Failed to execute \"%s\"\n", path);	\
	return 1;

int main(int argc, const char *argv[]) {
	// Initialize IPC pipes
	static int pipe_fd[2][2];
	pipe(pipe_fd[0]); // child1 -> child2
	pipe(pipe_fd[1]); // child2 -> master
	// Initialize children PID buffers
	pid_t children[2];
	// Initialize relative path
	char *dir = dirname((char *)argv[0]);
	// Three way fork
	if ((children[0] = fork()) == 0) {
		// ========== Child process #1 ==========
		PIPE_WR(pipe_fd[0]);
		PIPE_XX(pipe_fd[1]);
		EXEC(EXECUTABLE_1);
	} else if ((children[1] = fork()) == 0) {
		// ========== Child process #2 ==========
		PIPE_RD(pipe_fd[0]);
		PIPE_WR(pipe_fd[1]);
		EXEC(EXECUTABLE_2);
	} else {
		// =========== Master process ===========
		// Close irrelvant pipe ends
		PIPE_XX(pipe_fd[0]);
		close(pipe_fd[1][WR]);
		// Pipe into and out of child processes
		char *const buf = (char *)malloc(LINE_SIZE * sizeof(char));
		ssize_t buf_size = -1;
		do {
			// Pipe from child process 2 to out buffer
			if (buf_size) buf_size = read(pipe_fd[1][RD], buf, LINE_SIZE);
			if (buf_size > 0) write(STDOUT_FILENO, buf, buf_size);
#ifdef DEBUG
			printf("Read buffer size = %ld\n", buf_size);
#endif
		} while (buf_size);
		// Wait for child processes
		int stat;
		waitpid(children[0], &stat, 0);
		waitpid(children[1], &stat, 0);
	}
	return 0;
}
