/**
 * EEL5733 Assignments
 * @author Yuxuan Zhang (zhangyuxuan@ufl.edu)
 * @brief Macro Definations
 */
#ifndef STATISTICS_H
#define STATISTICS_H

#ifndef DEBUG_STAT
#define STAT(ID)
#define STAT_UPDATE(ID, ...)
#define STAT_REPORT(ID)
#else // DEBUG_STAT
typedef struct _STAT_ {
	size_t hit_count, total;
} _STAT_;

#define STAT_NAME(ID) _STAT_ ## ID ## _

#define STAT(ID) \
	_STAT_ STAT_NAME(ID) = {0, 0};

#define STAT_UPDATE(ID, VALID) {			\
	STAT_NAME(ID).hit_count +=	VALID;	\
	STAT_NAME(ID).total ++;				\
}

#define STAT_REPORT(ID) fprintf(							\
	stderr												,	\
	"Statistics:%s => %6.2f%% (%zu hits, %zu total)\n"	,	\
	#ID													,	\
	STAT_NAME(ID).total										\
		? 100 * ((float)STAT_NAME(ID).hit_count) /			\
		  ((float)STAT_NAME(ID).total)						\
		: (float)0										,	\
	STAT_NAME(ID).hit_count								,	\
	STAT_NAME(ID).total										\
)
#endif

#endif
