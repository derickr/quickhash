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
	int  fd;

	options->size = 1048576;
	options->check_for_dupes = 1;

	fd = open(argv[1], O_RDONLY);
	h = qhi_set_load_from_file(fd, options);

	printf("done loading\n");
	printf("exists: %d\n", qhi_set_exists(h, (qhv) 1623693378));
	printf("exists: %d\n", qhi_set_exists(h, (qhv) -418599962));
	printf("exists: %d\n", qhi_set_exists(h, (qhv) -966693999));
	printf("exists: %d\n", qhi_set_exists(h, (qhv) 113552487));
	printf("exists: %d\n", qhi_set_exists(h, (qhv) (int32_t) 0xE0878B35));
	printf("exists: %d\n", qhi_set_exists(h, (qhv) 0));
	printf("exists: %d\n", qhi_set_exists(h, (qhv) (int32_t) 0x5453494c));
	printf("done checking\n");

	qhi_free(h);
	qho_free(options);

	return 0;
}
