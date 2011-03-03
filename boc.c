#include <getopt.h>
#include <stdlib.h>

#include "boc.h"

int main(int argc, char * const * argv) {

	init();

	parseArgs(argc, argv);
	updateAllProcs();
	touchupFilters();
	listAllProcs();

	cleanup();

	return 0;
}

int parseArgs(int argc, char * const *argv) {
	int c;
	int option_index = 0;
	struct Filter *filter;
	
	static struct option long_options[] = {
		{"name", 1, 0, 'n'},
		{"pid", 1, 0, 'p'},
		{"regexp", 1, 0, 'r'},
		{0, 0, 0, 0},
	};

	while(1) {
		c = getopt_long(argc, argv, "n:p:r:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
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
			default:
				break;
		}
	}

	return 0;
}
