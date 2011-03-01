#include <stdlib.h>
#include <string.h>

#include "otop.h"

int init() {
	first_process = NULL;
	last_process = NULL;

	return 0;
}

struct Process *newProcess() {
	struct Process *proc;

	proc = malloc(sizeof(struct Process));
	memset(proc, 0, sizeof(struct Process));

	proc->next_process = NULL;
	proc->prev_process = last_process;

	if (first_process == NULL) {
		first_process = proc;
	}

	if (last_process) {
		last_process->next_process = proc;
	}

	last_process = proc;

	return proc;
}

int deleteProcess(struct Process *proc) {

	if (proc != first_process) {
		proc->prev_process->next_process = proc->next_process;
	} else {
		first_process = proc->next_process;
	}

	if (proc != last_process) {
		proc->next_process->prev_process = proc->prev_process;
	} else {
		last_process = proc->prev_process;
	}

	if (proc->has_commandline) {
		unsigned int i;

		for (i=0; i < proc->argc; i++) {
			free(proc->argv[i]);
		}
		free(proc->argv);

	}
	free(proc->name);

	free(proc);

	return 0;
}

int cleanup() {

	while(last_process) {
		deleteProcess(last_process);
	}

	return 0;
}
