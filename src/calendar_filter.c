/**
 * EEL5733 Assignment 4
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Calendar Filter 
 */
#include "stdio.h"
#include "tasks.h"
#include "util.h"

int main(int argc, const char *argv[]) {
	struct IO io = {
		.puts=&puts,
		.getline=&stdin_getline,
	};
	calendar_filter(&io);
	return 0;
}
