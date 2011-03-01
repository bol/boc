#include <stdio.h>

#include "otop.h"

int listAllProcs() {
	struct Process *proc;

	for (proc = first_process; (proc->next_process); proc = proc->next_process) {
		printf("%-7d %c %-12d\t%2d %2lu\t%-25s\n", proc->pid, proc->state,
		(proc->write_bytes - proc->cancelled_write_bytes),
		IOPRIO_PRIO_CLASS(proc->iopriority), IOPRIO_PRIO_DATA(proc->iopriority),
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
