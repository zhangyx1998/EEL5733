/**
 * EEL5733 Assignment 4
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Location Updater 
 */
#include "stdlib.h"
#include "macros.h"
#include "thread.h"
#include "tasks.h"

int main(int argc, const char *argv[]) {
#ifdef DEBUG_REDIRECT
	freopen("test/location_updater.1.in", "r", stdin);
#endif
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
