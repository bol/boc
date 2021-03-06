#include <stdlib.h>
#include <string.h>

#include "boc.h"

int init() {
	first_process = newProcess();
	last_process = first_process;

	first_filter = NULL;
	last_filter = NULL;

	return 0;
}

struct Filter *newFilter() {
	struct Filter *filter;

	filter = malloc(sizeof(struct Filter));
	memset(filter, 0, sizeof(struct Filter));

	filter->next_filter = NULL;
	filter->prev_filter = last_filter;

	if (first_filter == NULL) {
		first_filter = filter;
	}

	if(last_filter) {
		last_filter->next_filter = filter;
	}

	last_filter = filter;

	return filter;
}

int deleteFilter(struct Filter * filter) {
	if (filter != first_filter) {
		filter->prev_filter->next_filter = filter->next_filter;
	} else {
		first_filter = filter->next_filter;
	}

	if (filter != last_filter) {
		filter->next_filter->prev_filter = filter->prev_filter;
	} else {
		last_filter = filter->prev_filter;
	}

		regfree(&filter->preg);

	free(filter);

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

	while(last_filter) {
		deleteFilter(last_filter);
	}

	return 0;
}
