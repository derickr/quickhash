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
/* $Id: qh_inthash.c 533 2010-01-25 11:06:51Z derick $ */

#include "php.h"
#include "zend.h"
#include "zend_API.h"
#include "qh_inthash.h"
#include "quickhash.h"

zend_class_entry *qh_ce_inthash;

PHPAPI zend_class_entry *php_qh_get_inthash_ce(void)
{
	return qh_ce_inthash;
}

zend_object_handlers qh_object_handlers_inthash;

static void qh_object_free_storage_inthash(void *object TSRMLS_DC);
static zend_object_value qh_object_new_inthash(zend_class_entry *class_type TSRMLS_DC);

/* Reflection Information Structs */
ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_inthash_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, size)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_inthash_add, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_inthash_exists, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_inthash_get_value, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_inthash_load_from_file, 0, 0, 1)
	ZEND_ARG_INFO(0, filename)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_inthash_save_to_file, 0, 0, 1)
	ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_inthash_load_from_string, 0, 0, 1)
	ZEND_ARG_INFO(0, contents)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_inthash_save_to_string, 0, 0, 0)
ZEND_END_ARG_INFO()

/* Class methods definition */
zend_function_entry qh_funcs_inthash[] = {
	PHP_ME(QuickHashIntHash, __construct,    arginfo_qh_inthash_construct,        ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntHash, add,            arginfo_qh_inthash_add,              ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntHash, exists,         arginfo_qh_inthash_exists,           ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntHash, getValue,       arginfo_qh_inthash_get_value,        ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntHash, loadFromFile,   arginfo_qh_inthash_load_from_file,   ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntHash, saveToFile,     arginfo_qh_inthash_save_to_file,     ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntHash, loadFromString, arginfo_qh_inthash_load_from_string, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntHash, saveToString,   arginfo_qh_inthash_save_to_string,   ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void qh_register_class_inthash(TSRMLS_D)
{
	zend_class_entry ce_inthash;

	INIT_CLASS_ENTRY(ce_inthash, "QuickHashIntHash", qh_funcs_inthash);
	ce_inthash.create_object = qh_object_new_inthash;
	qh_ce_inthash = zend_register_internal_class_ex(&ce_inthash, NULL, NULL TSRMLS_CC);
	memcpy(&qh_object_handlers_inthash, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	zend_declare_class_constant_long(qh_ce_inthash, "CHECK_FOR_DUPES", sizeof("CHECK_FOR_DUPES") - 1, QH_NO_DUPLICATES TSRMLS_CC);
	zend_declare_class_constant_long(qh_ce_inthash, "DO_NOT_USE_ZEND_ALLOC",  sizeof("DO_NOT_USE_ZEND_ALLOC") - 1, QH_DO_NOT_USE_ZEND_ALLOC TSRMLS_CC);
	zend_declare_class_constant_long(qh_ce_inthash, "HASHER_NO_HASH",   sizeof("HASHER_NO_HASH") - 1,  QH_HASHER_NO_HASH TSRMLS_CC);
	zend_declare_class_constant_long(qh_ce_inthash, "HASHER_JENKINS1",  sizeof("HASHER_JENKINS1") - 1, QH_HASHER_JENKINS1 TSRMLS_CC);
	zend_declare_class_constant_long(qh_ce_inthash, "HASHER_JENKINS2",  sizeof("HASHER_JENKINS2") - 1, QH_HASHER_JENKINS2 TSRMLS_CC);
}

static inline zend_object_value qh_object_new_inthash_ex(zend_class_entry *class_type, php_qh_inthash_obj **ptr TSRMLS_DC)
{
	php_qh_inthash_obj *intern;
	zend_object_value retval;
	zval *tmp;

	intern = emalloc(sizeof(php_qh_inthash_obj));
	memset(intern, 0, sizeof(php_qh_inthash_obj));
	if (ptr) {
		*ptr = intern;
	}

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	zend_hash_copy(intern->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
	
	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) qh_object_free_storage_inthash, NULL TSRMLS_CC);
	retval.handlers = &qh_object_handlers_inthash;
	
	return retval;
}

static zend_object_value qh_object_new_inthash(zend_class_entry *class_type TSRMLS_DC)
{
	return qh_object_new_inthash_ex(class_type, NULL TSRMLS_CC);
}

static void qh_object_free_storage_inthash(void *object TSRMLS_DC)
{
	php_qh_inthash_obj *intern = (php_qh_inthash_obj *) object;

	if (intern->hash) {
		qho *tmp_options = intern->hash->options;

		qhi_free(intern->hash);
		qho_free(tmp_options);
	}

	zend_object_std_dtor(&intern->std TSRMLS_CC);
	efree(object);
}


static void process_flags(qho *options, long flags)
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

static int qh_inthash_initialize(php_qh_inthash_obj *obj, long size, long flags TSRMLS_DC)
{
	qho *options = qho_create();

	options->size = size;
	process_flags(options, flags);

	obj->hash = qhi_create(options);
	if (obj->hash == NULL) {
		qho_free(options);
		return 0;
	}
	return 1;
}

/* {{{ proto QuickHashIntHash QuickHashIntHash::__construct( int size, [ int options] )
   Creates a new QuickHashIntHash */
PHP_METHOD(QuickHashIntHash, __construct)
{
	long size;
	long options = 0;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &size, &options) == SUCCESS) {
		if (!qh_inthash_initialize(zend_object_store_get_object(getThis() TSRMLS_CC), size, options TSRMLS_CC)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not initialize hash.");
		}
	}
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
}
/* }}} */

/* {{{ proto bool QuickHashIntHash::add( int key, int value )
   Adds an element with key key and value value to the hash */
PHP_METHOD(QuickHashIntHash, add)
{
	zval              *object;
	php_qh_inthash_obj *inthash_obj;
	long               key, value;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oll", &object, qh_ce_inthash, &key, &value) == FAILURE) {
		RETURN_FALSE;
	}
	inthash_obj = (php_qh_inthash_obj *) zend_object_store_get_object(object TSRMLS_CC);
	RETURN_BOOL(qhi_hash_add(inthash_obj->hash, key, value));
}
/* }}} */

/* {{{ proto bool QuickHashIntHash::exists( int key )
   Tests whether the element with key key is part of the hash */
PHP_METHOD(QuickHashIntHash, exists)
{
	zval              *object;
	php_qh_inthash_obj *inthash_obj;
	long               key;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &object, qh_ce_inthash, &key) == FAILURE) {
		RETURN_FALSE;
	}
	inthash_obj = (php_qh_inthash_obj *) zend_object_store_get_object(object TSRMLS_CC);
	RETURN_BOOL(qhi_set_exists(inthash_obj->hash, key));
}
/* }}} */

/* {{{ proto int QuickHashIntHash::getValue( int key )
   Returns the value of they key if it exists, or NULL otherwise */
PHP_METHOD(QuickHashIntHash, getValue)
{
	zval               *object;
	php_qh_inthash_obj *inthash_obj;
	long                key;
	uint32_t            value;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &object, qh_ce_inthash, &key) == FAILURE) {
		RETURN_FALSE;
	}
	inthash_obj = (php_qh_inthash_obj *) zend_object_store_get_object(object TSRMLS_CC);
	if (qhi_hash_get_value(inthash_obj->hash, key, &value)) {
		RETURN_LONG(value);
	}
}
/* }}} */

static uint32_t qh_inthash_initialize_from_file(php_qh_inthash_obj *obj, php_stream *stream, long flags TSRMLS_DC)
{
	php_stream_statbuf finfo;
	uint32_t           nr_of_elements, elements_read = 0;
	uint32_t           bytes_read;
	int32_t            key_buffer[1024];
	qho               *options = qho_create();

	// deal with options
	process_flags(options, flags);

	// obtain the filesize
	if (php_stream_stat(stream, &finfo) != 0) {
		qho_free(options);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not obtain file information");
		return 0;
	}

	// check whether we have a real file (and not a directory or something)
	if (!S_ISREG(finfo.sb.st_mode)) {
		qho_free(options);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "File is not a normal file");
		return 0;
	}

	// if the filesize is not an increment of 8, abort
	if (finfo.sb.st_size % 8 != 0) {
		qho_free(options);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "File is in the wrong format (not a multiple of 8 bytes)");
		return 0;
	}
	nr_of_elements = finfo.sb.st_size / 8;

	// override the nr of bucket lists as we know better
	options->size = nr_of_elements;

	// create the hash
	obj->hash = qhi_create(options);
	if (obj->hash == NULL) {
		qho_free(options);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Couldn't create hash");
		return 0;
	}

	// read the elements and add them to the hash
	do {
		bytes_read = php_stream_read(stream, (char*)&key_buffer, sizeof(key_buffer));
		qhi_hash_add_elements_from_buffer(obj->hash, key_buffer, bytes_read / 4);
		elements_read += (bytes_read / 8);
	} while (elements_read < nr_of_elements);
	return nr_of_elements;
}

/* {{{ proto QuickHashIntHash QuickHashIntHash::loadFromFile( string filename [, int options ] )
   Creates a QuickHashIntHash from data in file filename */
PHP_METHOD(QuickHashIntHash, loadFromFile)
{
	char *filename;
	int   filename_len;
	long  options = 0;
	php_stream *stream;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &filename, &filename_len, &options) == FAILURE) {
		return;
	}

	qh_instantiate(qh_ce_inthash, return_value TSRMLS_CC);
	stream = php_stream_open_wrapper(filename, "r", IGNORE_PATH | REPORT_ERRORS, NULL);
	if (stream) {
		qh_inthash_initialize_from_file(zend_object_store_get_object(return_value TSRMLS_CC), stream, options);
		php_stream_close(stream);
	}
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
}
/* }}} */

int qh_inthash_save_to_file(php_stream *stream, php_qh_inthash_obj *obj)
{
	uint32_t    idx;
	uint32_t    elements_in_buffer = 0;
	int32_t     key_buffer[1024];
	qhi        *hash = obj->hash;

	for (idx = 0; idx < hash->bucket_count; idx++)	{
		qhl *list = &(hash->bucket_list[idx]);
		qhb *p = list->head;
		qhb *n;

		if (p) {
			while(p) {
				n = p->next;

				key_buffer[elements_in_buffer] = p->key;
				key_buffer[elements_in_buffer + 1] = hash->values[p->value_idx];
				elements_in_buffer += 2;

				if (elements_in_buffer == 512) {
					if (php_stream_write(stream, (char*)key_buffer, elements_in_buffer * 4) != (elements_in_buffer * 4)) {
						return 0;
					}
					elements_in_buffer = 0;
				}

				p = n;
			}
		}
	}

	if (elements_in_buffer > 0) {
		if (php_stream_write(stream, (char*)key_buffer, elements_in_buffer * 4) != (elements_in_buffer * 4)) {
			return 0;
		}
	}
	return 1;
}

/* {{{ proto void QuickHashIntHash::saveToFile( string filename )
   Saves the hash to a file */
PHP_METHOD(QuickHashIntHash, saveToFile)
{
	char *filename;
	int   filename_len;
	zval              *object;
	php_qh_inthash_obj *inthash_obj;
	php_stream *stream;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &object, qh_ce_inthash, &filename, &filename_len) == FAILURE) {
		return;
	}

	inthash_obj = (php_qh_inthash_obj *) zend_object_store_get_object(object TSRMLS_CC);
	stream = php_stream_open_wrapper(filename, "w", IGNORE_PATH | REPORT_ERRORS, NULL);

	if (stream) {
		qh_inthash_save_to_file(stream, inthash_obj);
		php_stream_close(stream);
	}
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
}
/* }}} */

static uint32_t qh_inthash_initialize_from_string(php_qh_inthash_obj *obj, char *contents, long length, long flags TSRMLS_DC)
{
	uint32_t  nr_of_elements;
	qho      *options = qho_create();

	// deal with options
	process_flags(options, flags);

	// if the size is not an increment of 4, abort
	if (length % 8 != 0) {
		qho_free(options);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "String is in the wrong format (not a multiple of 8 bytes)");
		return 0;
	}
	nr_of_elements = length / 4;

	// override the nr of bucket lists as we know better
	options->size = nr_of_elements;

	// create the hash
	obj->hash = qhi_create(options);
	if (obj->hash == NULL) {
		qho_free(options);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Couldn't create hash");
		return 0;
	}

	// read the elements and add them to the hash
	qhi_hash_add_elements_from_buffer(obj->hash, (int32_t*)contents, nr_of_elements);
	return nr_of_elements;
}

/* {{{ proto QuickHashIntHash QuickHashIntHash::loadFromString( string contents [, int options ] )
   Creates a QuickHashIntHash from data in a string */
PHP_METHOD(QuickHashIntHash, loadFromString)
{
	char    *contents;
	int      contents_len;
	long     options = 0;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &contents, &contents_len, &options) == FAILURE) {
		return;
	}

	qh_instantiate(qh_ce_inthash, return_value TSRMLS_CC);
	qh_inthash_initialize_from_string(zend_object_store_get_object(return_value TSRMLS_CC), contents, contents_len, options);
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
}
/* }}} */

#define PHP_QH_BUFFER_GROWTH 1024

char *qh_inthash_save_to_string(uint32_t *string_len, php_qh_inthash_obj *obj)
{
	uint32_t    idx;
	uint32_t    elements_in_buffer = 0;
	int32_t    *key_buffer;
	qhi        *hash = obj->hash;

	key_buffer = emalloc(PHP_QH_BUFFER_GROWTH + 4);

	for (idx = 0; idx < hash->bucket_count; idx++)	{
		qhl *list = &(hash->bucket_list[idx]);
		qhb *p = list->head;
		qhb *n;

		if (p) {
			while(p) {
				n = p->next;

				key_buffer[elements_in_buffer] = p->key;
				key_buffer[elements_in_buffer + 1] = hash->values[p->value_idx];
				elements_in_buffer += 2;

				if (elements_in_buffer % PHP_QH_BUFFER_GROWTH == 0) {
					key_buffer = erealloc(key_buffer, elements_in_buffer + PHP_QH_BUFFER_GROWTH + 4);
				}

				p = n;
			}
		}
	}

	key_buffer[elements_in_buffer] = 0;
	*string_len = elements_in_buffer * 4;
	return (char*)key_buffer;
}

/* {{{ proto string QuickHashIntHash::saveToString()
   Returns the hash as a string */
PHP_METHOD(QuickHashIntHash, saveToString)
{
	zval              *object;
	php_qh_inthash_obj *inthash_obj;
	char              *string;
	uint32_t           string_len;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, qh_ce_inthash) == FAILURE) {
		return;
	}

	inthash_obj = (php_qh_inthash_obj *) zend_object_store_get_object(object TSRMLS_CC);

	string = qh_inthash_save_to_string(&string_len, inthash_obj);
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
	RETURN_STRINGL(string, string_len, 0);
}
/* }}} */
