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
	int32_t value;
	
	options->size = 2000;
	options->check_for_dupes = 1;
	h = qhi_create(options);

	printf("exists: %d\n", (int32_t) qhi_set_exists(h, 7));
	qhi_set_add(h, 1);
	qhi_hash_add(h, 7, 1048575);
	qhi_set_add(h, 9);
	qhi_set_add(h, 15);
	qhi_hash_add(h, 17, 912312);
	qhi_set_add(h, 54);
	printf("exists: %d\n", (int32_t) qhi_set_exists(h, 7));
	printf("exists: %d\n", (int32_t) qhi_set_exists(h, 17));
	printf("exists: %d\n", (int32_t) qhi_set_exists(h, 53));

	// values
	if (qhi_hash_get(h, 7, &value)) {
		printf("value: %d = %d\n", 7, value);
	}
	if (qhi_hash_get(h, 17, &value)) {
		printf("value: %d = %d\n", 17, value);
	}
	if (qhi_hash_get(h, 53, &value)) {
		printf("value: %d = %d\n", 53, value);
	}

	qhi_free(h);
	qho_free(options);

	return 0;
}
