#include "quickhash.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	qhi *h;
	qho *options = qho_create();
	int  fd;

	options->size = 1048576;
	options->check_for_dupes = 1;

	fd = open(argv[1], O_RDONLY);
	h = qhi_set_load_from_file(fd, options);
	close(fd);

	printf("done loading\n");
	printf("exists: %d\n", qhi_set_exists(h, 1623693378));
	printf("exists: %d\n", qhi_set_exists(h, -418599962));
	printf("exists: %d\n", qhi_set_exists(h, -966693999));
	printf("exists: %d\n", qhi_set_exists(h, 113552487));
	printf("exists: %d\n", qhi_set_exists(h, 0xE0878B35));
	printf("exists: %d\n", qhi_set_exists(h, 0));
	printf("exists: %d\n", qhi_set_exists(h, 0x5453494c));
	printf("done checking\n");

	fd = open("/tmp/test-save", O_WRONLY | O_CREAT);
	qhi_set_save_to_file(fd, h);

	qhi_free(h);
	qho_free(options);

	return 0;
}
