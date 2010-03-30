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

int main(void)
{
	qhi *h;
	qho *options = qho_create();
	
	options->size = 2000;
	options->check_for_dupes = 1;
	h = qhi_create(options);

	printf("exists: %d\n", qhi_set_exists(h, (qhv) 7));
	qhi_set_add(h, (qhv) 1);
	qhi_set_add(h, (qhv) 7);
	qhi_set_add(h, (qhv) 9);
	qhi_set_add(h, (qhv) 15);
	qhi_set_add(h, (qhv) 17);
	qhi_set_add(h, (qhv) 54);
	printf("exists: %d\n", qhi_set_exists(h, (qhv) 7));
	printf("exists: %d\n", qhi_set_exists(h, (qhv) 17));
	printf("exists: %d\n", qhi_set_exists(h, (qhv) 53));

	qhi_free(h);
	qho_free(options);

	return 0;
}
