/**
 * EEL5733 Assignment 3
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Account Data Structure Header
 */
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "stdlib.h"
#include "pthread.h"

typedef unsigned int AccountId;
typedef unsigned long CashValue;

typedef struct {
	AccountId id;
	CashValue balance;
	pthread_mutex_t *mutex;
	pthread_cond_t *transaction;
} * Account;

static inline Account account(AccountId id, CashValue balance) {
	Account a = malloc(sizeof(*a));
	a->id = id;
	a->balance = balance;
	a->mutex = malloc(sizeof(*(a->mutex)));
	a->transaction = malloc(sizeof(*(a->transaction)));
	pthread_mutex_init(a->mutex, NULL);
	pthread_cond_init(a->transaction, NULL);
	return a;
}

static inline void _account(Account a) {
	pthread_mutex_destroy(a->mutex);
	pthread_cond_destroy(a->transaction);
	free(a->mutex);
	free(a->transaction);
	free((void *)a);
}

#endif
