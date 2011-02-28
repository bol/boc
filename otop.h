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
		char name[1024];
		char state;
		unsigned int pid;
		unsigned int ppid;
		unsigned int pgrp;
		unsigned int session;
		unsigned int tty_nr;
		unsigned int tpgid;
		unsigned int rchar;
		unsigned int wchar;

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
int listAllProcs();

/* update.c */
int updateAllProcs();
int updateProc(int);
int parseStat(struct Process *, int);
int parseIo(struct Process *, int);
int parseCmdLine(struct Process *, int);


#endif // __OTOP_H
