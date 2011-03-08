/* We want the GNU basename */
#define _GNU_SOURCE

#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>

#include "boc.h"

int colouriseProcs() {
	struct Filter *filter;
	struct Process *proc;

	for (proc = first_process; (proc->next_process); proc = proc->next_process) {
		if ( !TESTOPT(OPT_KERNEL) && (proc->size == 0)) {
			continue;
		}

		if (first_filter == NULL ) {
			proc->colour = 1;
			continue;
		}

		for(filter = first_filter; (filter); filter = filter->next_filter) {
			if ((filter->pid > 0) && filter->pid == proc->pid) {
				colourise(proc);
				continue;
			} else if ((filter->name) && 0 == strcmp(filter->name,
						(proc->has_commandline? fullArgv(proc):proc->name))) {
				colourise(proc);
				continue;
			} else if ((filter->regular_expression) && 0 == regexec(&filter->preg,
						(proc->has_commandline? fullArgv(proc):proc->name), 0, 0, 0)) {
				colourise(proc);
			}
		}
	}

	return 0;
}

int colourise(struct Process *proc) {
	struct Process *p, *q;
	
	proc->colour = 1;

	p = proc->parent;
	while(p) {
		p->colour = 2;
		p = p->parent;
	}

	p = proc->first_child;
	while(p) {
		q = p;
		while (q) {
			q->colour = 2;
			q = q->next_sibling;
		}
		p = p->first_child;
	}

	return 0;

}

int listAllProcs() {
	struct Process *proc;

	listHeader();
	for (proc = first_process; (proc->next_process); proc = proc->next_process) {
		if (proc->colour == 1 ) {
			listProc(proc);
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

	if (TESTOPT(OPT_FULL)) {
		for (i=1; i < proc->argc; i++) {
			c += sprintf(c, " %s", proc->argv[i]);
		}
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
	int depth;

	depth = 0;

	proc = first_process;
	while (1) {
		if (proc->colour != 0) {
			treePrint(proc, depth);
		}

		if (proc->first_child) {
			proc = proc->first_child;
			depth++;
			continue;
		} else if (proc->next_sibling) {
			proc = proc->next_sibling;
			continue;
		} else {
			while (proc->next_sibling == NULL) {
				if (proc->parent == NULL) {
					return 0;
				}
				proc = proc->parent;
				depth--;
			}

			proc = proc->next_sibling;
		}
	}

	return -1;
}

int treePrint(struct Process *proc, int depth) {
	struct Process *parent;
	int i, j;

	for ( i=depth; i>0; i-- ) {
		if ( i==1 ) {
				if(proc->next_sibling && proc->next_sibling->colour) {
					printf("  |-");
				} else {
					printf("  `-");
				}
		} else {
			parent = proc->parent;
			for ( j=i; j>2; j-- ) {
					parent = parent->parent;
			}
			if (parent->next_sibling && parent->next_sibling->colour) {
				printf("  |");
			} else {
				printf("   ");
			}
		}
	}
	printf("%s\n", (proc->has_commandline? fullArgv(proc):proc->name) );

	return 0;
}
