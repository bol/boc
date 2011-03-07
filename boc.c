#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include "boc.h"

int main(int argc, char * const * argv) {

	init();

	parseArgs(argc, argv);
	updateAllProcs();
	touchupFilters();
	touchupProcs();
	colouriseProcs();
	if (TESTOPT(OPT_TREE)) {
			drawTree();
	} else {
			listAllProcs();
	}

	cleanup();

	return 0;
}

int parseArgs(int argc, char * const *argv) {
	int c;
	int option_index = 0;
	struct Filter *filter;
	
	static struct option long_options[] = {
		{"full", 0, 0, 'f'},
		{"help", 0, 0, 'h'},
		{"io", 0, 0, 'i'},
		{"kernel", 0, 0, 'k'},
		{"memory", 0, 0, 'm'},
		{"name", 1, 0, 'n'},
		{"pid", 1, 0, 'p'},
		{"regexp", 1, 0, 'r'},
		{"tree", 0, 0, 't'},
		{"user", 0, 0, 'u'},
		{0, 0, 0, 0},
	};

	while(1) {
		c = getopt_long(argc, argv, "fhkimn:p:r:tu", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
			case 'f':
				SETOPT(OPT_FULL);
				break;
			case 'h':
				usage(argv);
				exit(0);
			case 'i':
				SETOPT(OPT_IO);
				break;
			case 'k':
				SETOPT(OPT_KERNEL);
				break;
			case 'm':
				SETOPT(OPT_MEM);
				break;
			case 'n':
				filter = newFilter();
				filter->name = optarg;
				break;
			case 'r':
				filter = newFilter();
				filter->regular_expression = optarg;
				break;
			case 'p':
				filter = newFilter();
				filter->pid = atoi(optarg);
				break;
			case 't':
				SETOPT(OPT_TREE);
				break;
			case 'u':
				SETOPT(OPT_USER);
				break;
			default:
				break;
		}
	}

	return 0;
}

int usage(char * const * argv) {
	printf("Usage: %s [ -f ] [ -i ] [ -k ] [ -m ] [ -u ] [-p PID ] [ -n NAME ] [ -r REGEXP ]\n", argv[0] );
	printf("       %s -t [ -f ]\n", argv[0] );
	printf("Display a list or tree of processes.\n\n");
	printf("\nGeneral options:\n");
	printf("  -h,%-15s show this help message", "--help\n");
	printf("  -t,%-15s draw a process tree", "--tree\n");
	printf("\nInformation options:\n");
	printf("  -f,%-15s include command line arguments in the process name", "--full\n");
	printf("  -i,%-15s show information about process io\n", "--io\n");
	printf("  -k,%-15s show kernel processes\n", "--kernel\n");
	printf("  -m,%-15s show information about process memory usage\n", "--memory\n");
	printf("  -u,%-15s show information about process ownership\n", "--user\n");
	printf("\nSearch options:\n");
	printf("  -n NAME,%-15s display processes whose name matches NAME\n", "--name=NAME\n");
	printf("  -p PID,%-15s display processes whose pid matches PID\n", "--pid=PID\n");
	printf("  -r REGEXP,%-15s display processes whose name matches REGEXP\n", "--regexp=REGEXP\n");
	printf("\n");

	return 0;
}
