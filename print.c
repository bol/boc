#include <stdio.h>

#include "boc.h"

int listAllProcs() {
	struct Process *proc;

	for (proc = first_process; (proc->next_process); proc = proc->next_process) {
		printf("%-7d %c %-12d\t%-3ld %d %lu\t%-6d %-6d\t%-25s\n", proc->pid, proc->state,
		(proc->write_bytes - proc->cancelled_write_bytes),
		proc->nice, IOPRIO_PRIO_CLASS(proc->iopriority), IOPRIO_PRIO_DATA(proc->iopriority),
		proc->euid, proc->egid,
		(proc->has_commandline? fullArgv(proc):proc->name));
	}

	return 0;
}

char * fullArgv(struct Process *proc) {
	static char buf[1024];
	char *c;
	unsigned int i;

	c = buf;
	c += sprintf(c, "%s", proc->argv[0]);
	for (i=1; i < proc->argc; i++) {
			c += sprintf(c, " %s", proc->argv[i]);
	}

	return buf;
}
