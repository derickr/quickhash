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
#include "qh_intset.h"
#include "qh_inthash.h"
#include "qh_intstringhash.h"
#include "qh_stringinthash.h"

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

int32_t php_qh_get_size_from_string(void *context)
{
	php_qh_string_context *ctxt = (php_qh_string_context*) context;

	return ctxt->string_len;
}

int php_qh_load_int32t_from_string_func(void *context, int32_t *buffer, uint32_t elements)
{
	int32_t req;
	php_qh_string_context *ctxt = (php_qh_string_context*) context;
#if 0
printf("req: %d; pos: %d : length: %d, max: %d\n",
	elements * sizeof(int32_t),
	ctxt->ptr - ctxt->string, 
	ctxt->string_len, (ctxt->string_len - (ctxt->ptr - ctxt->string)));
#endif
	req = elements * sizeof(int32_t);
	if (req > (ctxt->string_len - (ctxt->ptr - ctxt->string))) {
		req = (ctxt->string_len - (ctxt->ptr - ctxt->string));
	}

	memcpy(buffer, ctxt->ptr, req);
	ctxt->ptr += req;

	return req / sizeof(int32_t);
}

int php_qh_load_chars_from_string_func(void *context, char *buffer, uint32_t elements)
{
	php_qh_string_context *ctxt = (php_qh_string_context*) context;

	memcpy(buffer, ctxt->ptr, elements);
	ctxt->ptr += elements;

	return elements;
}

int php_qh_save_int32t_to_string_func(void *context, int32_t *buffer, uint32_t elements)
{
	php_qh_string_context *ctxt = (php_qh_string_context*) context;

	ctxt->string = erealloc(ctxt->string, ctxt->string_len + (elements * sizeof(int32_t)) + 1);
	memcpy(ctxt->string + ctxt->string_len, buffer, elements * sizeof(int32_t));
	ctxt->string_len += (elements * sizeof(int32_t));
	ctxt->string[ctxt->string_len] = '\0';

	return 1;
}

int php_qh_save_chars_to_string_func(void *context, char *buffer, uint32_t elements)
{
	php_qh_string_context *ctxt = (php_qh_string_context*) context;

	ctxt->string = erealloc(ctxt->string, ctxt->string_len + elements + 1);
	memcpy(ctxt->string + ctxt->string_len, buffer, elements);
	ctxt->string_len += elements;
	ctxt->string[ctxt->string_len] = '\0';

	return 1;
}

int32_t php_qh_get_size_from_stream(void *context)
{
	php_qh_stream_context *ctxt = (php_qh_stream_context*) context;
	php_stream_statbuf     finfo;

	// obtain the filesize
	if (php_stream_stat(ctxt->stream, &finfo) == 0) {
		return finfo.sb.st_size;
	}
	return -1;
}

int php_qh_load_int32t_from_stream_func(void *context, int32_t *buffer, uint32_t elements)
{
	php_qh_stream_context *ctxt = (php_qh_stream_context*) context;
	TSRMLS_FETCH();

	return php_stream_read(ctxt->stream, (char*)buffer, elements * sizeof(int32_t)) / sizeof(int32_t);
}

int php_qh_load_chars_from_stream_func(void *context, char *buffer, uint32_t elements)
{
	php_qh_stream_context *ctxt = (php_qh_stream_context*) context;
	TSRMLS_FETCH();

	return php_stream_read(ctxt->stream, buffer, elements);
}

int php_qh_save_int32t_to_stream_func(void *context, int32_t *buffer, uint32_t elements)
{
	php_qh_stream_context *ctxt = (php_qh_stream_context*) context;
	TSRMLS_FETCH();

	if (php_stream_write(ctxt->stream, (char*)buffer, elements * sizeof(int32_t)) != (elements * sizeof(int32_t))) {
		return 0;
	}
	return 1;
}

int php_qh_save_chars_to_stream_func(void *context, char *buffer, uint32_t elements)
{
	php_qh_stream_context *ctxt = (php_qh_stream_context*) context;
	TSRMLS_FETCH();

	if (php_stream_write(ctxt->stream, buffer, elements) != elements) {
		return 0;
	}
	return 1;
}

/**
 * Does some tests on the stream to see whether we can use it for reading data from.
 */
int php_qh_prepare_file(qhi **hash, qho *options, php_stream *stream, long size, long flags, php_qh_stream_validator validator, uint32_t *nr_of_elements, uint32_t *value_array_length TSRMLS_DC)
{
	php_stream_statbuf finfo;

	// deal with options
	qh_process_flags(options, flags);

	// obtain the filesize
	if (php_stream_stat(stream, &finfo) != 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not obtain file information");
		return 0;
	}

	// check whether we have a real file (and not a directory or something)
	if (!S_ISREG(finfo.sb.st_mode)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "File is not a normal file");
		return 0;
	}

	// run the validator
	if (!validator(finfo, stream, nr_of_elements, value_array_length)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "File is in the wrong format");
		return 0;
	}

	// automatically set the size if the size is still 0.
	options->size = size == 0 ? *nr_of_elements : size;

	return 1;
}

/**
 * Does some tests on the string to see whether we can use it for reading data from.
 */
int php_qh_prepare_string(qhi **hash, qho *options, char *string, long length, long size, long flags, php_qh_string_validator validator, uint32_t *nr_of_elements, uint32_t *value_array_length TSRMLS_DC)
{
	// deal with options
	qh_process_flags(options, flags);

	// run the validator
	if (!validator(string, length, nr_of_elements, value_array_length)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "String is in the wrong format");
		return 0;
	}

	// automatically set the size if the size is still 0.
	options->size = size == 0 ? *nr_of_elements : size;

	return 1;
}


PHP_MINIT_FUNCTION(quickhash)
{
	ZEND_INIT_MODULE_GLOBALS(quickhash, quickhash_init_globals, NULL);

	qh_register_class_intset(TSRMLS_C);
	qh_register_class_inthash(TSRMLS_C);
	qh_register_class_intstringhash(TSRMLS_C);
	qh_register_class_stringinthash(TSRMLS_C);

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
