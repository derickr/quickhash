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
#include <unistd.h>

int main(void)
{
	qhi *h;
	qho *options = qho_create();
	int32_t value = 0;
	
	options->size = 4;
	options->check_for_dupes = 1;
	options->key_type = QHI_KEY_TYPE_STRING;
	h = qhi_create(options);

	printf("%d\n", qhi_set_exists(h, (qhv) "eight hundred seventy six"));
	printf("%d\n", qhi_hash_add(h, (qhv) "eight hundred seventy six", (qhv) 899));
	printf("%d\n", qhi_set_exists(h, (qhv) "eight hundred seventy six"));
	printf("%d\n", qhi_hash_get(h, (qhv) "eight hundred seventy six", (qhv*) &value));
	printf("value %d\n", value);
	value = 0;
	printf("%d\n", qhi_set_delete(h, (qhv) "eight hundred seventy six"));
	printf("%d\n", qhi_set_exists(h, (qhv) "eight hundred seventy six"));
	printf("%d\n", qhi_hash_get(h, (qhv) "eight hundred seventy six", (qhv*) &value));
	printf("value %d\n", value);

	qhi_free(h);
	qho_free(options);
	return 0;
}
