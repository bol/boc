#include <stdlib.h>

#include "boc.h"

int touchupFilters () {
	struct Filter *filter;

	for (filter = first_filter; (filter); filter = filter->next_filter) {
		if (filter->regular_expression) {
			regcomp(&filter->preg, filter->regular_expression, REG_NOSUB);
		}
	}

	return 0;
}

int touchupProcs () {
	struct Process *proc;
	struct Process *pproc;

	for (proc = first_process; proc->next_process; proc = proc->next_process) {
		pproc = getProcByPid(proc->ppid);

		if (pproc) {
			addChildProc(pproc, proc);
		}
	}
	
	return 0;
}

int addChildProc(struct Process * parent, struct Process * child) {
	struct Process *tmp_proc;

	if (parent->first_child == NULL) {
		parent->first_child = child;
	} else {
		tmp_proc = parent->first_child;

		while(tmp_proc->next_sibling) {
			tmp_proc = tmp_proc->next_sibling;
		}
		tmp_proc->next_sibling = child;
	}
	child->parent = parent;

	return 0;
}

struct Process * getProcByPid(unsigned int pid) {
	struct Process *proc;

	for (proc = first_process; (proc->next_process); proc = proc->next_process) {
		if (proc->pid == pid) {
			return proc;
		}
	}

	return NULL;
}
