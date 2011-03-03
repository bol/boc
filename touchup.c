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
