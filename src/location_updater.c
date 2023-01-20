#include "macros.h"
#include "util.h"
#include "unistd.h"

static int pipe_fd[3][2];

#define PIPE_RD(ID) pipe_fd[ID][0]
#define PIPE_WR(ID) pipe_fd[ID][1]
#define PIPE_SETUP(ID1, ID2)	\
	dup2(PIPE_RD(ID1), stdin);	\
	close(PIPE_RD(ID1));		\
	dup2(PIPE_WR(ID2), stdout);	\
	close(PIPE_WR(ID2));

#define EXEC_EMAIL_FILTER "./email_filter.bin"
#define EXEC_CAL_UPDATER "./calendar_updater.bin"

int main(int argc, const char *argv[]) {
	// Initialize IPC pipes
	pipe(pipe_fd[0]); // master -> child1
	pipe(pipe_fd[1]); // child1 -> child2
	pipe(pipe_fd[2]); // child2 -> master
	// Initialize children PID buffers
	pid_t children[2];

	char* args[] = {"./hello.bin"};
	execvp(args[0], args);
	// Three way fork
	if (!(children[0] = fork())) {
		// Child process #1
		printf("I am the first child.\n");
		// PIPE_SETUP(0, 2);
	} else if (!(children[1] = fork())) {
		// Child process #2
		// PIPE_SETUP(1, 2);
		printf("I am the second child.\n");
	} else {
		// Master process
		// char * const buf = malloc(BUFFER_SIZE * sizeof(char));
		// char * wr_ptr = buf, rd_ptr = buf;
		// while (1) {

		// 	printf("%s", buf);
		// }
		// waitpid(children[0]);
		// waitpid(children[1]);
		wait(NULL);
		printf("I am the master! My children are: %d, %d\n", children[0], children[1]);
	}
	return 0;
}
