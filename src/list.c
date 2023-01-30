/**
 * EEL5733 Assignment 1
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief List operations
 */
#include "stdlib.h"
#include "macros.h"
#include "string.h"
#include "list.h"
#include "util.h"

struct Node static * list = NULL;

struct Node *parse(char *buf) {
	// Meeting   ,01/13/2019,10:30,NEB102____
	struct Node *node = malloc(sizeof(struct Node));
	// Parse date
	node->date.year = atoi(&buf[17]);
	node->date.month = atoi(&buf[11]);
	node->date.day = atoi(&buf[14]);
	// Parse time
	node->time.hours = atoi(&buf[22]);
	node->time.minutes = atoi(&buf[25]);
	memcpy(node->name, &buf[0], 10);
	memcpy(node->loc, &buf[28], 10);
	node->next = NULL;
	return node;
}

int comp_date(struct Node *a, struct Node *b) {
	if (a == NULL || b == NULL) return -1;
	if (a->date.year != b->date.year) 
		return a->date.year - b->date.year;
	else if (a->date.month != b->date.month)
		return a->date.month - b->date.month;
	else
		return a->date.day - b->date.day;
}

int comp_time(struct Node *a, struct Node *b) {
	if (a == NULL || b == NULL) return -1;
	if (a->time.hours != b->time.hours) 
		return a->time.hours - b->time.hours;
	else
		return a->time.minutes - b->time.minutes;
}

int comp(struct Node *a, struct Node *b) {
	int date_diff = comp_date(a, b);
	if (date_diff) return date_diff;
	else return comp_time(a, b);
}

void print_node(struct Node * node, unsigned int valid) {
	if (valid) {
		printf(
			"%02d/%02d/%04d,%02d:%02d,%.10s\n",
			node->date.month,
			node->date.day,
			node->date.year,
			node->time.hours,
			node->time.minutes,
			node->loc
		);
	} else {
		printf(
			"%02d/%02d/%04d,--:--,NA\n",
			node->date.month,
			node->date.day,
			node->date.year
		);
	}
}

#define CHECK_PRINT(P, N)							\
	if (P == NULL && N != NULL) print_node(N, 1);	\
	else if (N != NULL && comp_date(P, N) < 0) {	\
		print_node(N, 1);							\
	}

int list_create(struct Node *node) {
	struct Node *prev = NULL;
	unsigned int index = 0;
	if (list == NULL) {
		// Initialize list
		list = node;
		print_node(node, 1);
		return index;
	}
	// List Search: find insertion point
	for (
		struct Node * p = list;
		p != NULL;
		prev = p, p = p->next, index++
	) {
		// Termination condition: found event later than current
		if (comp(node, p) <= 0) {
			// Insert before current node
			if (prev == NULL) {
				// Current node has no ancestor
				node->next = list;
				list = node;
			} else {
				// Normal insertion
				node->next = p;
				prev->next = node;
			}
			// Print updated event information
			CHECK_PRINT(prev, node);
			return index;
		}
	}
	// If control reaches this point, no event later than the
	// current node was scheduled.
	prev->next = node;
	if (comp_date(prev, node)) print_node(node, 1);
	return ++index;
}

int list_delete(struct Node *node) {
	struct Node *prev = NULL;
	unsigned int index = 0;
	if (list == NULL) {
		// Trying to modify an empty list
		EPRINT("Error: trying to delete from an empty list\n");
		return -1;
	}
	// List Search: find node to be deleted
	for (
		struct Node *p = list;
		p != NULL;
		prev = p, p = p->next, index++
	) {
		// Termination condition: found event with given name
		if (memcmp(p, node, sizeof(struct Node) - sizeof(void *)) == 0) {
			// Delete the node
			if (prev != NULL) prev->next = p->next;
			else list = p->next;
			// Check if it is necessary to print updates
			if (comp_date(prev, node) < 0) {
				// Deleted node is the leading node of the same day
				if (comp_date(node, p->next) < 0) {
					// No other same-day event registered
					print_node(p, 0);
				} else {
					print_node(p->next, 1);
				}
			}
			return index;
			// Free up allocated space
			free(p);
			free(node);
		}
	}
	// If control reaches this point, no event with the exact
	// same name as the given one can be found.
	EPRINT("Error: unable to find event with name \"%.10s\"", node->name);
	if (comp_date(prev, node)) print_node(node, 1);
	return ++index;
}

int list_modify(struct Node *node) {
	struct Node *prev = NULL;
	unsigned int index = 0;
	if (list == NULL) {
		// Trying to modify an empty list
		EPRINT("Error: trying to modify an empty list\n");
		return -1;
	}
	// List Search: find replacement node
	for (
		struct Node * p = list;
		p != NULL;
		prev = p, p = p->next, index++
	) {
		// Termination condition: found event with given name
		if (memcmp(p->name, node->name, 10) == 0) {
			int diff[2] = {comp(prev, node), comp(node, p->next)};
			// Check if in-place modification is possible
			if (diff[0] <= 0 && diff[1] <= 0) {
				// In-place modify current node
				if (prev != NULL) prev->next = node;
				else list = node;
				node->next = p->next;
				CHECK_PRINT(prev, node);
			} else {
				// Remove current node
				if (prev != NULL) prev->next = p->next;
				else list = p->next;
				CHECK_PRINT(prev, p->next);
				free(p);
				// Re-scan the list to insert modified node
				list_create(node);
			}
			return index;
		}
	}
	// If control reaches this point, no event with the exact
	// same name as the given one can be found.
	EPRINT("Error: unable to find event with name \"%.10s\"", node->name);
	if (comp_date(prev, node)) print_node(node, 1);
	return ++index;
}
