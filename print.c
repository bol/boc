#include <stdio.h>

#include "otop.h"

int listAllProcs() {
	struct Process *proc;

	for (proc = first_process; (proc->next_process); proc = proc->next_process) {

		printf("%c\t%d\t%d\t%s\t\t%d\n", proc->state, proc->pid, proc->ppid, proc->name, (proc->write_bytes - proc->cancelled_write_bytes));
	}

	return 0;
}
