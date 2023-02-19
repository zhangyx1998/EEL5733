/**
 * EEL5733 Assignment 3
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Standalone Test Program For Parser
 */
#include "stdio.h"
#include "parser.h"

int main(int argc, const char *argv[]) {
	char *buf = NULL;
	size_t capacity = 0;
	while (getline(&buf, &capacity, stdin) >= 0) {
		Instruction i = parse(buf);
		printf(
			"%c src=%u dst=%u amt=%lu\n",
			(char)i->type,
			i->account_src,
			i->account_dst,
			i->amount
		);
		free(i);
	}
	return 0;
}
