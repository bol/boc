
#include "boc.h"

int main(int argc, char ** argp) {

	init();

	updateAllProcs();
	listAllProcs();

	cleanup();

	return 0;
}

