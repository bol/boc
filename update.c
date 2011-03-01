#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "otop.h"

int updateAllProcs() {
	DIR *DIRP;
	struct dirent *direntp;
	int pid;

	/* FIXME: Should not assume /proc*/
	DIRP = opendir("/proc");

	while ((direntp = readdir(DIRP))) {
		pid = atoi(direntp->d_name);
		if (pid == 0) {
			continue;
		}

		updateProc(pid);
	}
	closedir(DIRP);

	return 0;
}

int updateProc(int pid) {
	char path[256];
	int fd;
	struct Process *proc;

	proc = newProcess();
	proc->pid = pid;
	
	/* FIXME: Should not assume /proc*/
	sprintf(path, "/proc/%i/stat", pid);
	fd = open(path, O_RDONLY);
	parseStat(proc, fd);
	close(fd);

	sprintf(path, "/proc/%i/io", pid);
	fd = open(path, O_RDONLY);
	parseIo(proc, fd);
	close(fd);

	sprintf(path, "/proc/%i/cmdline", pid);
	fd = open(path, O_RDONLY);
	parseCmdLine(proc, fd);
	close(fd);

	getIOPrio(proc);

	return 0;
}

int parseStat(struct Process *proc, int fd) {
	char buf[1024];

	read(fd, buf, sizeof(buf));

	sscanf(buf, "%d %ms %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %*d %llu %lu %ld %lu %lu %lu %lu %lu %lu %*u %*u %*u %*u %lu %*u %*u %d %d %u %u %llu %lu %ld",
					&proc->pid,
					&proc->name,
					&proc->state,
					&proc->ppid,
					&proc->pgrp,
					&proc->session,
					&proc->tty_nr,
					&proc->tpgid,
					&proc->flags,
					&proc->minflt,
					&proc->cminflt,
					&proc->majflt,
					&proc->cmajflt,
					&proc->utime,
					&proc->stime,
					&proc->cutime,
					&proc->cstime,
					&proc->priority,
					&proc->nice,
					&proc->numthreads,
					&proc->starttime,
					&proc->vsize,
					&proc->rss,
					&proc->rsslim,
					&proc->startcode,
					&proc->endcode,
					&proc->startstack,
					&proc->kstkesp,
					&proc->kstkeip,
					&proc->wchan,
					&proc->exit_signal,
					&proc->processor,
					&proc->rt_priority,
					&proc->policy,
					&proc->delayacct_blkio_ticks,
					&proc->guest_time,
					&proc->cguest_time);

	return 0;
}

int parseIo(struct Process *proc, int fd) {
	char buf[1024];

	read(fd, buf, sizeof(buf));
	sscanf(buf, "rchar: %d\nwchar: %d\nsyscr: %d\nsyscw: %d\nread_bytes: %d\nwrite_bytes: %d\ncancelled_write_bytes: %d", 
					&proc->rchar,
					&proc->wchar,
					&proc->syscr,
					&proc->syscw,
					&proc->read_bytes,
					&proc->write_bytes,
					&proc->cancelled_write_bytes);

	return 0;
}

int parseCmdLine(struct Process *proc, int fd) {
	char buf[1024];
	unsigned int buflen, i, len;
	char *c;

	buflen = read(fd, buf, sizeof(buf));

	if(buflen == 0) {
			return 0;
	}
	proc->has_commandline = 1;

	/* cmdline is not null terminated if there are no command line arguments. */
	if (buf[buflen-1] != '\0') {
		buf[buflen++] = '\0';
	}

	/* figure out argc */
	proc->argc=0;
	for (i=0; i < buflen; i++) {
		if (buf[i] == '\0'){
			proc->argc++;
		}
		c++;
	}

	/* One for each pointer plus null */
	proc->argv = malloc(2 * proc->argc * sizeof(char *));
	proc->argv[proc->argc] = NULL;

	/* And the char arrays */
	c = buf;
	for (i=0; i < proc->argc; i++) {
		len = strlen(c)+1;
		proc->argv[i] = malloc(len);
		strncpy(proc->argv[i], c, len);

		while (*c != '\0'){
				c++;
		}
		c++;
	}

	return 0;
}

int getIOPrio(struct Process *proc) {
	proc->iopriority = syscall(SYS_ioprio_get, IOPRIO_WHO_PROCESS, proc->pid);

	return 0;
}
