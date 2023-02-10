/**
 * EEL5733 Assignment 2
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Location Updater 
 */
#include "stdlib.h"
#include "macros.h"
#include "thread.h"
#include "tasks.h"

int main(int argc, const char *argv[]) {
	size_t thread_buf_size = argc < 2
		? 10
		: atoi(argv[1]);
	launch(
		&email_filter,
		&calendar_filter,
		thread_buf_size
	);
	return 0;
}
