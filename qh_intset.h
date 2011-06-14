/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2011 StumbleUpon Inc.                             |
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

PHP_METHOD(QuickHashIntSet, __construct);
PHP_METHOD(QuickHashIntSet, getSize);
PHP_METHOD(QuickHashIntSet, add);
PHP_METHOD(QuickHashIntSet, exists);
PHP_METHOD(QuickHashIntSet, delete);
PHP_METHOD(QuickHashIntSet, loadFromFile);
PHP_METHOD(QuickHashIntSet, saveToFile);
PHP_METHOD(QuickHashIntSet, loadFromString);
PHP_METHOD(QuickHashIntSet, saveToString);

void qh_register_class_intset(TSRMLS_D);
PHPAPI zend_class_entry *php_qh_get_intset_ce(void);

#endif
