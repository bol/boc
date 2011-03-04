#include <getopt.h>
#include <stdlib.h>

#include "boc.h"

int main(int argc, char * const * argv) {

	init();

	parseArgs(argc, argv);
	updateAllProcs();
	touchupFilters();
	touchupProcs();
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
		{"io", 0, 0, 'i'},
		{"kernel", 0, 0, 'k'},
		{"mem", 0, 0, 'm'},
		{"name", 1, 0, 'n'},
		{"pid", 1, 0, 'p'},
		{"regexp", 1, 0, 'r'},
		{"tree", 0, 0, 't'},
		{"user", 0, 0, 'u'},
		{0, 0, 0, 0},
	};

	while(1) {
		c = getopt_long(argc, argv, "kimn:p:r:tu", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
			case 'k':
				SETOPT(OPT_KERNEL);
				break;
			case 'i':
				SETOPT(OPT_IO);
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
