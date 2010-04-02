/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010 StumbleUpon Inc.                                  |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Derick Rethans <derick@derickrethans.nl>                    |
   +----------------------------------------------------------------------+
 */
#include "quickhash.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	qhi *h;
	qho *options = qho_create();
	int  fd, i;
	int32_t value;

	options->size = 1048576;
	options->check_for_dupes = 1;

	fd = open("../tests/small.hash", O_RDONLY);
	h = qhi_hash_load_from_file(fd, options);

	printf("done loading\n");
	for (i = 0; i < 40; i++) {
		if (qhi_set_exists(h, (qhv) i)) {
			qhi_hash_get(h, (qhv) i, (qhv*) &value);
			printf("%03d; value: %04d\n", i, value);
		}
	}
	printf("done checking\n");

	qhi_free(h);
	qho_free(options);

	return 0;
}
