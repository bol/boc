#include <stdio.h>
#include <string.h>

#include "boc.h"

int listAllProcs() {
	struct Filter *filter;
	struct Process *proc;

	for (proc = first_process; (proc->next_process); proc = proc->next_process) {
		if (first_filter == NULL ) {
			listProc(proc);
			continue;
		}

		for(filter = first_filter; (filter); filter = filter->next_filter) {
			if ((filter->pid > 0) && filter->pid == proc->pid) {
				listProc(proc);
				continue;
			} else if ((filter->name) && 0 == strcmp(filter->name,
						(proc->has_commandline? fullArgv(proc):proc->name))) {
				listProc(proc);
				continue;
			} else if ((filter->regular_expression) && 0 == regexec(&filter->preg,
						(proc->has_commandline? fullArgv(proc):proc->name), 0, 0, 0)) {
						
				listProc(proc);
			}
		}
	}

	return 0;
}

int listProc(struct Process * proc) {
		printf("%-7d %c %-12d\t%-3ld %d %lu\t%-6d %-6d\t%-25s\n", proc->pid, proc->state,
		(proc->write_bytes - proc->cancelled_write_bytes),
		proc->nice, IOPRIO_PRIO_CLASS(proc->iopriority), IOPRIO_PRIO_DATA(proc->iopriority),
		proc->euid, proc->egid,
		(proc->has_commandline? fullArgv(proc):proc->name));

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
