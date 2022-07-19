/*
   +----------------------------------------------------------------------+
   | PHP                                                                  |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2011 StumbleUpon Inc.                             |
   | Copyright (c) 2012-2022 Derick Rethans                               |
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
/* $Id: qh_stringinthash.h 588 2010-03-03 21:34:00Z derick $ */

#ifndef PHP_QUICKHASH_STRINGINTHASH_H
#define PHP_QUICKHASH_STRINGINTHASH_H

#include "lib/quickhash.h"
#include "quickhash.h"

typedef struct _php_qh_stringinthash_obj php_qh_stringinthash_obj;

struct _php_qh_stringinthash_obj {
	QH_PHP_OBJ
};

PHP_METHOD(QuickHashStringIntHash, __construct);
PHP_METHOD(QuickHashStringIntHash, add);
PHP_METHOD(QuickHashStringIntHash, exists);
PHP_METHOD(QuickHashStringIntHash, get);
PHP_METHOD(QuickHashStringIntHash, set);
PHP_METHOD(QuickHashStringIntHash, update);
PHP_METHOD(QuickHashStringIntHash, delete);
PHP_METHOD(QuickHashStringIntHash, arrayAccessSet);
PHP_METHOD(QuickHashStringIntHash, arrayAccessUnset);
PHP_METHOD(QuickHashStringIntHash, loadFromFile);
PHP_METHOD(QuickHashStringIntHash, saveToFile);
PHP_METHOD(QuickHashStringIntHash, loadFromString);
PHP_METHOD(QuickHashStringIntHash, saveToString);

void qh_register_class_stringinthash(TSRMLS_D);
PHPAPI zend_class_entry *php_qh_get_stringinthash_ce(void);

#endif
