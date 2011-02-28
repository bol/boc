#ifndef __OTOP_H
#define __OTOP_H

#include <stdbool.h>

/* structs */
struct Process {
	/* list pointers */
	struct Process * next_process;
	struct Process * prev_process;

	bool has_commandline;

	/* cmdline */
	unsigned int argc;
	char ** argv;

	/* stat */
	char *name;
	char state;
	unsigned int pid;
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

	/* io */
	unsigned int syscr;
	unsigned int syscw;
	unsigned int read_bytes;
	unsigned int write_bytes;
	unsigned int cancelled_write_bytes;
};

/* global variables */
struct Process * first_process;
struct Process * last_process;

/* function prototypes */

/* mem.c */
int init();
struct Process * newProcess();
int deleteProcess(struct Process *);
int cleanup();

/* print.c */
char *fullArgv(struct Process *);
int listAllProcs();

/* update.c */
int updateAllProcs();
int updateProc(int);
int parseStat(struct Process *, int);
int parseIo(struct Process *, int);
int parseCmdLine(struct Process *, int);

#endif // __OTOP_H
