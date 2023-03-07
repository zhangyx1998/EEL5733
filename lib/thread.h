/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Thread headers
 */
#ifndef THREAD_H
#define THREAD_H

#include "util.h"

#define THREAD_BUF_SIZE 10

typedef void (*ThreadEntry)(const struct IO * const);

void launch(ThreadEntry, ThreadEntry, size_t);

#endif
