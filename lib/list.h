/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief List headers
 */
#ifndef LIST_H
#define LIST_H

#include "util.h"
struct Date {
	int year;
	int month;
	int day;
};

struct Time {
	int hours;
	int minutes;
};

struct Node {
	struct Date date;
	struct Time time;
	char name[10];
	char loc[10];
	struct Node *next;
};

struct Node *parse(char *buf);
int list_create(struct Node *node, const struct IO * const);
int list_delete(struct Node *node, const struct IO * const);
int list_modify(struct Node *node, const struct IO * const);

#endif
