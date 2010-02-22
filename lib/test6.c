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
	uint32_t i;
	int32_t value;
	
	options->size = 500000;
	options->check_for_dupes = 1;
	h = qhi_create(options);

	for (i= 0; i < 1048576; i += 2) {
		qhi_hash_add(h, i, i*3 + i % 7);
	}

	for (i= 0; i < 1048576; i++) {
		if (qhi_hash_get(h, i, &value)) {
//			printf("value: %d = %d\n", i, value);
		} else {
//			printf("value: %d = ?\n", i);
		}
	}

	qhi_free(h);
	qho_free(options);

	return 0;
}
