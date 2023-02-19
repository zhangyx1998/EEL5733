/**
 * EEL5733 Assignment 3
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Standalone Test Program For "Vector"
 */
#include "stdio.h"
#include "string.h"
#include "vector.h"
#include "macros.h"

int main(int argc, const char *argv[]) {
	char *buf = NULL;
	size_t capacity = 0;
	Vector vec = vector();
	while (getline(&buf, &capacity, stdin) >= 0) {
		size_t l = strlen(buf);
		if (l <= 2) {
			EPRINT("Invalid line: %s", buf);
			continue;
		}
		DEBUG_PRINT("%.*s", (int)(strlen(buf) - 1), buf);
		char *s = NULL;
		int i;
		switch (buf[0]) {
			case '+':
				s = malloc((l - 1) * sizeof(*s));
				memcpy((void *)s, (void *)(buf + 2), l - 2);
				s[l - 2] = 0;
				vector_push(vec, s);
				break;
			case '-':
				i = atoi(buf + 2);
				s = vector_delete(vec, (size_t)i);
				fputs(s, stdout);
				free((void *)s);
				break;
			default:
				EPRINT("bad format: %s", s);
		}
	}
	// Indicate end of file
	puts("====>> EOF <<====");
	// Reverse output all lines in stack
	while (vector_len(vec)) {
		char *s = vector_pop(vec);
		fputs(s, stdout);
		DEBUG_PRINT("<< \"%.*s\"", (int)(strlen(s) - 1), s);
		free(s);
	}
	return 0;
}
