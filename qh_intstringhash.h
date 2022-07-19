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
/* $Id: qh_intstringhash.h 588 2010-03-03 21:34:00Z derick $ */

#ifndef PHP_QUICKHASH_INTSTRINGHASH_H
#define PHP_QUICKHASH_INTSTRINGHASH_H

#include "lib/quickhash.h"
#include "quickhash.h"

typedef struct _php_qh_intstringhash_obj php_qh_intstringhash_obj;

struct _php_qh_intstringhash_obj {
	QH_PHP_OBJ
};

PHP_METHOD(QuickHashIntStringHash, __construct);
PHP_METHOD(QuickHashIntStringHash, add);
PHP_METHOD(QuickHashIntStringHash, get);
PHP_METHOD(QuickHashIntStringHash, set);
PHP_METHOD(QuickHashIntStringHash, arrayAccessSet);
PHP_METHOD(QuickHashIntStringHash, update);
PHP_METHOD(QuickHashIntStringHash, delete);
PHP_METHOD(QuickHashIntStringHash, loadFromFile);
PHP_METHOD(QuickHashIntStringHash, saveToFile);
PHP_METHOD(QuickHashIntStringHash, loadFromString);
PHP_METHOD(QuickHashIntStringHash, saveToString);

void qh_register_class_intstringhash(TSRMLS_D);
PHPAPI zend_class_entry *php_qh_get_intstringhash_ce(void);

#endif
