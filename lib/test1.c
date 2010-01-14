#include "quickhash.h"
#include <stdio.h>

int main(void)
{
	qhi *h;
	qho *options = qho_create();
	
	options->size = 2000;
	options->check_for_dupes = 1;
	h = qhi_create(options);

	printf("exists: %d\n", qhi_set_exists(h, 7));
	qhi_set_add(h, 1);
	qhi_set_add(h, 7);
	qhi_set_add(h, 9);
	qhi_set_add(h, 15);
	qhi_set_add(h, 17);
	qhi_set_add(h, 54);
	printf("exists: %d\n", qhi_set_exists(h, 7));
	printf("exists: %d\n", qhi_set_exists(h, 17));
	printf("exists: %d\n", qhi_set_exists(h, 53));

	qhi_free(h);
	qho_free(options);

	return 0;
}
