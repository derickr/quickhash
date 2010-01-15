/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2010 The PHP Group                                |
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
/* $Id$ */

#ifndef PHP_QUICKHASH_INTSET_H
#define PHP_QUICKHASH_INTSET_H

#include "lib/quickhash.h"

typedef struct _php_qh_intset_obj php_qh_intset_obj;

struct _php_qh_intset_obj {
	zend_object   std;
	qhi          *hash;
};

/* Bitfield for hash options */
#define QH_NO_DUPLICATES          0x0001
#define QH_DO_NOT_USE_ZEND_ALLOC  0x0002

#define QH_HASHER_NO_HASH         0x0100
#define QH_HASHER_JENKINS1        0x0200
#define QH_HASHER_JENKINS2        0x0400
#define QH_HASHER_MASK            0xFF00

PHP_METHOD(QuickHashIntSet, __construct);
PHP_METHOD(QuickHashIntSet, add);
PHP_METHOD(QuickHashIntSet, exists);
PHP_METHOD(QuickHashIntSet, loadFromFile);
PHP_METHOD(QuickHashIntSet, saveToFile);

void qh_register_class_intset(TSRMLS_D);

#endif
