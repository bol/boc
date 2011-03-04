/* We want the GNU basename */
#define _GNU_SOURCE

#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>

#include "boc.h"

int listAllProcs() {
	struct Filter *filter;
	struct Process *proc;

	listHeader();
	for (proc = first_process; (proc->next_process); proc = proc->next_process) {
		if ( !TESTOPT(OPT_KERNEL) && (proc->ppid == 0|| proc->size == 0)) {
			continue;
		}

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

int listHeader() {
		printf("%-7s %c ", "PID", 'S');
		if (TESTOPT(OPT_USER)) {
			printf("%-10s %-10s ", "USER", "GROUP");
		}
		if (TESTOPT(OPT_IO)) {
			printf("%-10s %-10s", "READ", "WRITE");
		}
		if (TESTOPT(OPT_MEM)) {
			printf("%-10s %-10s %-10s %-10s %-10s ", "SIZE", "RESIDENT", "SHARE", "TEXT", "DATA");
		}
		printf("%-25s\n", "NAME");

		return 0;
}

int listProc(struct Process * proc) {
		printf("%-7d %c ", proc->pid, proc->state);
		if (TESTOPT(OPT_USER)) {
			printf("%-10s %-10s ", uidToName(proc->euid), gidToName(proc->egid));
		}
		if (TESTOPT(OPT_IO)) {
			printf("%-10d %-10d", proc->read_bytes/1024, (proc->write_bytes - proc->cancelled_write_bytes)/1024);
		}
		if (TESTOPT(OPT_MEM)) {
			printf("%-10d %-10d %-10d %-10d %-10d ", proc->size, proc->resident, proc->share, proc->text, proc->data);
		}
		printf("%-25s\n", (proc->has_commandline? fullArgv(proc):proc->name));

		return 0;
}

char * fullArgv(struct Process *proc) {
	static char buf[1024];
	char *c;
	unsigned int i;

	c = buf;
	c += sprintf(c, "%s", ((proc->argv[0][0] == '/')? basename(proc->argv[0]):proc->argv[0]) );
	for (i=1; i < proc->argc; i++) {
			c += sprintf(c, " %s", proc->argv[i]);
	}

	return buf;
}

char * uidToName(uid_t uid) {
	struct passwd *passwd;

	passwd = getpwuid(uid);

	return passwd->pw_name;
}

char * gidToName(gid_t gid) {
	struct group *group;

	group = getgrgid(gid);

	return group->gr_name;
}

int drawTree() {
	struct Process *proc;
	unsigned int depth, i;

	depth = 0;

	proc = first_process;
	while (1) {
		for (i=depth;i>0;i--) {
			if (i==1) {
				printf("  |-");
			} else {
				printf("  |");
			}
		}
		printf("%s\n", (proc->has_commandline? fullArgv(proc):proc->name) );

		if (proc->first_child) {
			proc = proc->first_child;
			depth++;
			continue;
		}

		if (proc->next_sibling) {
			proc = proc->next_sibling;
			continue;
		}

		while (proc->next_sibling == NULL) {
			if (proc->parent == NULL) {
				return 0;
			}
			proc = proc->parent;
			depth--;
		}

		proc = proc->next_sibling;
	}

	return -1;
}
