#include <sys/types.h>
#include <sys/stat.h>
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
	
	/* FIXME: Should not assume /proc*/
	sprintf(path, "/proc/%i/stat", pid);
	fd = open(path, O_RDONLY);
	parseStat(proc, fd);
	close(fd);

	sprintf(path, "/proc/%i/cmdline", pid);
	fd = open(path, O_RDONLY);
	parseCmdLine(proc, fd);
	close(fd);

	return 0;
}

int parseStat(struct Process *proc, int fd) {
	char buf[1024];

	read(fd, buf, sizeof(buf));

	sscanf(buf, "%d %s %c %d %d %d %d %d",
					&proc->pid,
					proc->name,
					&proc->state,
					&proc->ppid,
					&proc->pgrp,
					&proc->session,
					&proc->tty_nr,
					&proc->tpgid);

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

	printf("%d\t%s", proc->argc, proc->argv[0]);
	for (i=1; i < proc->argc; i++) {
			printf(" %s", proc->argv[i]);
	}
	printf("\n");

	return 0;
}
