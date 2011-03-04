#ifndef __OTOP_H
#define __OTOP_H

#include <sys/types.h>
#include <regex.h>
#include <stdbool.h>

/* ioprio stuff stolen from ioprio.h in the linux kernel */
enum { IOPRIO_WHO_PROCESS = 1 };

enum {
        IOPRIO_CLASS_NONE,
        IOPRIO_CLASS_RT,
        IOPRIO_CLASS_BE,
        IOPRIO_CLASS_IDLE,
};

#define IOPRIO_CLASS_SHIFT      (13)
#define IOPRIO_PRIO_MASK        ((1UL << IOPRIO_CLASS_SHIFT) - 1)

#define IOPRIO_PRIO_CLASS(mask) ((mask) >> IOPRIO_CLASS_SHIFT)
#define IOPRIO_PRIO_DATA(mask)  ((mask) & IOPRIO_PRIO_MASK)


/* global variables */
struct Process * first_process;
struct Process * last_process;

struct Filter * first_filter;
struct Filter * last_filter;
unsigned int options;


enum {
	OPT_KERNEL	= 1,
	OPT_TREE	= 2,
};

#define SETOPT(opt)	(options |= opt)
#define TESTOPT(opt)	((options & opt) == opt)


/* structs */
struct Process {
	/* list pointers */
	struct Process * next_process;
	struct Process * prev_process;

	struct Process * first_child;
	struct Process * next_sibling;
	struct Process * parent;

	bool has_commandline;
	unsigned int pid;
	int iopriority;

	/* cmdline */
	unsigned int argc;
	char ** argv;

	/* stat */
	char *name;
	char state;
	unsigned int ppid;
	unsigned int pgrp;
	unsigned int session;
	unsigned int tty_nr;
	unsigned int tpgid;
	unsigned int rchar;
	unsigned int wchar;
	unsigned int flags;
	unsigned long minflt;
	unsigned long cminflt;
	unsigned long majflt;
	unsigned long cmajflt;
	unsigned long utime;
	unsigned long stime;
	long int cutime;
	long int cstime;
	long int priority;
	long int nice;
	long int numthreads;
	unsigned long long starttime;
	unsigned long vsize;
	long int rss;
	unsigned long rsslim;
	unsigned long startcode;
	unsigned long endcode;
	unsigned long startstack;
	unsigned long kstkesp;
	unsigned long kstkeip;
	unsigned long wchan;
	int exit_signal;
	int processor;
	unsigned int rt_priority;
	unsigned int policy;
	unsigned long long delayacct_blkio_ticks;
	unsigned long guest_time;
	long int cguest_time;							

	/* status */
	int ruid;
	int euid;
	int ssuid;
	int fsuid;
	int rgid;
	int egid;
	int ssgid;
	int fsgid;

	/* statm */
	unsigned int size;
	unsigned int resident;
	unsigned int share;
	unsigned int text;
	unsigned int data;

	/* io */
	unsigned int syscr;
	unsigned int syscw;
	unsigned int read_bytes;
	unsigned int write_bytes;
	unsigned int cancelled_write_bytes;

};

struct Filter {
	/* list pointers */
	struct Filter * next_filter;
	struct Filter * prev_filter;

	unsigned int pid;
	char * name;
	char * regular_expression;
	regex_t preg;
};

/* function prototypes */

/* boc.c */
int parseArgs(int, char * const *);

/* mem.c */
int init();
struct Filter * newFilter();
struct Process * newProcess();
int deleteProcess(struct Process *);
int cleanup();

/* print.c */
char *fullArgv(struct Process *);
char *uidToName(uid_t uid);
char *gidToName(gid_t gid);
int drawTree();
int listAllProcs();
int listProc(struct Process *);

/* touchup.c */
int touchupFilters();
int touchupProcs();
struct Process * getProcByPid(unsigned int);
int addChildProc(struct Process*, struct Process *);

/* update.c */
int updateAllProcs();
int updateProc(int);
int parseStat(struct Process *, int);
int parseStatm(struct Process *, int);
int parseStatus(struct Process *, int);
int parseIo(struct Process *, int);
int parseCmdLine(struct Process *, int);
int getIOPrio(struct Process *);

#endif // __OTOP_H
