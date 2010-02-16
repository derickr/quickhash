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
/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "zend.h"
#include "zend_alloc.h"
#include "php_quickhash.h"
#include "php_globals.h"
#include "ext/standard/info.h"

#include "lib/quickhash.h"
#include "quickhash.h"

function_entry quickhash_functions[] = {
	{NULL, NULL, NULL}
};


zend_module_entry quickhash_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"quickhash",
	quickhash_functions,
	PHP_MINIT(quickhash),
	PHP_MSHUTDOWN(quickhash),
	PHP_RINIT(quickhash),	
	PHP_RSHUTDOWN(quickhash),
	PHP_MINFO(quickhash),
#if ZEND_MODULE_API_NO >= 20010901
	"0.0.1",
#endif
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_QUICKHASH
ZEND_GET_MODULE(quickhash)
#endif

ZEND_DECLARE_MODULE_GLOBALS(quickhash)
/*
PHP_INI_BEGIN()
PHP_INI_END()
*/
 
static void quickhash_init_globals(zend_quickhash_globals *quickhash_globals)
{
	/* Empty */
}

zval *qh_instantiate(zend_class_entry *pce, zval *object TSRMLS_DC)
{
	Z_TYPE_P(object) = IS_OBJECT;
	object_init_ex(object, pce);
	Z_SET_REFCOUNT_P(object, 1);
	Z_UNSET_ISREF_P(object);
	return object;
}

void qh_add_constants(zend_class_entry *ce TSRMLS_DC)
{
	zend_declare_class_constant_long(ce, "CHECK_FOR_DUPES", sizeof("CHECK_FOR_DUPES") - 1, QH_NO_DUPLICATES TSRMLS_CC);
	zend_declare_class_constant_long(ce, "DO_NOT_USE_ZEND_ALLOC",  sizeof("DO_NOT_USE_ZEND_ALLOC") - 1, QH_DO_NOT_USE_ZEND_ALLOC TSRMLS_CC);
	zend_declare_class_constant_long(ce, "HASHER_NO_HASH",   sizeof("HASHER_NO_HASH") - 1,  QH_HASHER_NO_HASH TSRMLS_CC);
	zend_declare_class_constant_long(ce, "HASHER_JENKINS1",  sizeof("HASHER_JENKINS1") - 1, QH_HASHER_JENKINS1 TSRMLS_CC);
	zend_declare_class_constant_long(ce, "HASHER_JENKINS2",  sizeof("HASHER_JENKINS2") - 1, QH_HASHER_JENKINS2 TSRMLS_CC);
}

void qh_process_flags(qho *options, long flags)
{
	options->check_for_dupes = (flags & QH_NO_DUPLICATES);
	if ((flags & QH_DO_NOT_USE_ZEND_ALLOC) == 0) {
		qh_set_memory_functions(options);
	}
	// if no hasher selected, pick jenkins1 as default
	if ((flags & QH_HASHER_MASK) == 0) {
		options->hasher = qha_jenkins1;
	} else {
		if (flags & QH_HASHER_NO_HASH) {
			options->hasher = qha_no_hash;
		} else if (flags & QH_HASHER_JENKINS1) {
			options->hasher = qha_jenkins1;
		} else if (flags & QH_HASHER_JENKINS2) {
			options->hasher = qha_jenkins2;
		}
	}
}


static void *qh_malloc(size_t size)
{
	return emalloc(size);
}

static void *qh_calloc(size_t nmemb, size_t size)
{
	return ecalloc(nmemb, size);
}

static void *qh_realloc(void *ptr, size_t size)
{
	return erealloc(ptr, size);
}

static void qh_free(void *ptr)
{
	if (ptr == NULL) {
		abort();
	}
	return efree(ptr);
}

void qh_set_memory_functions(qho *options)
{
	options->memory.malloc = qh_malloc;
	options->memory.realloc = qh_realloc;
	options->memory.calloc = qh_calloc;
	options->memory.free = qh_free;
}

PHP_MINIT_FUNCTION(quickhash)
{
	ZEND_INIT_MODULE_GLOBALS(quickhash, quickhash_init_globals, NULL);

	qh_register_class_intset(TSRMLS_C);
	qh_register_class_inthash(TSRMLS_C);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(quickhash)
{
	return SUCCESS;
}



PHP_RINIT_FUNCTION(quickhash)
{
	return SUCCESS;
}



PHP_RSHUTDOWN_FUNCTION(quickhash)
{
	return SUCCESS;
}


PHP_MINFO_FUNCTION(quickhash)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "quickhash support", "enabled");
	php_info_print_table_end();
}
