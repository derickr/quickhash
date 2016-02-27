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

#ifndef PHP_QUICKHASH_INTHASH_H
#define PHP_QUICKHASH_INTHASH_H

#include "lib/quickhash.h"

typedef struct _php_qh_inthash_obj php_qh_inthash_obj;

struct _php_qh_inthash_obj {
	qhi          *hash;
	zend_object   std;
};

PHP_METHOD(QuickHashIntHash, add);
PHP_METHOD(QuickHashIntHash, get);
PHP_METHOD(QuickHashIntHash, set);
PHP_METHOD(QuickHashIntHash, update);
PHP_METHOD(QuickHashIntHash, delete);
PHP_METHOD(QuickHashIntHash, loadFromFile);
PHP_METHOD(QuickHashIntHash, saveToFile);
PHP_METHOD(QuickHashIntHash, loadFromString);
PHP_METHOD(QuickHashIntHash, saveToString);

void qh_register_class_inthash(TSRMLS_D);
PHPAPI zend_class_entry *php_qh_get_inthash_ce(void);

#endif
