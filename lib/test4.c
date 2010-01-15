#include "quickhash.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	uint32_t i;

	for (i = 0; i < (1<<20) + 3; i++) {
		uint32_t s = qhi_normalize_size(i);
		printf("%9d -> %9d (mask = %08X)\n", i, s, s-1);
	}

	return 0;
}
