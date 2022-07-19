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
/* $Id: php_quickhash.h,v 1.9 2009-03-30 18:22:15 derick Exp $ */

#ifndef PHP_QUICKHASH_H
#define PHP_QUICKHASH_H

#include "php.h"

extern zend_module_entry quickhash_module_entry;
#define phpext_quickhash_ptr &quickhash_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(quickhash);
PHP_MSHUTDOWN_FUNCTION(quickhash);
PHP_RINIT_FUNCTION(quickhash);
PHP_RSHUTDOWN_FUNCTION(quickhash);
PHP_MINFO_FUNCTION(quickhash);

ZEND_BEGIN_MODULE_GLOBALS(quickhash)
	/* empty */
ZEND_END_MODULE_GLOBALS(quickhash)

#ifdef ZTS
# if PHP_VERSION_ID < 70000
#  define QUICKHASH_G(v) TSRMG(quickhash_globals_id, zend_quickhash_globals *, v)
# else
# if defined(COMPILE_DL_QUICKHASH)
ZEND_TSRMLS_CACHE_EXTERN();
# endif
#  define QUICKHASH_G(v) ZEND_TSRMG(quickhash_globals_id, zend_quickhash_globals *, v)
# endif
#else
# define QUICKHASH_G(v) (quickhash_globals.v)
#endif

#endif
