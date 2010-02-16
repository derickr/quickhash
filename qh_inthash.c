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
#include "qh_intset.h"
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

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_inthash_add, 0, 0, 1)
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
	PHP_ME(QuickHashIntSet,  __construct,    arginfo_qh_inthash_construct,        ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntHash, add,            arginfo_qh_inthash_add,              ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet,  exists,         arginfo_qh_inthash_exists,           ZEND_ACC_PUBLIC)
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
	qh_ce_inthash = zend_register_internal_class_ex(&ce_inthash, php_qh_get_intset_ce(), NULL TSRMLS_CC);
	memcpy(&qh_object_handlers_inthash, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	qh_add_constants(qh_ce_inthash);
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

/* {{{ proto bool QuickHashIntHash::add( int key [ , int value ] )
   Adds an element with key key and value value to the hash */
PHP_METHOD(QuickHashIntHash, add)
{
	zval              *object;
	php_qh_inthash_obj *inthash_obj;
	long               key, value = 1;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol|l", &object, qh_ce_inthash, &key, &value) == FAILURE) {
		RETURN_FALSE;
	}
	inthash_obj = (php_qh_inthash_obj *) zend_object_store_get_object(object TSRMLS_CC);
	RETURN_BOOL(qhi_hash_add(inthash_obj->hash, key, value));
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
	qh_process_flags(options, flags);

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
	qhi                           *hash = obj->hash;
	php_qh_save_to_stream_context  ctxt;

	ctxt.stream = stream;

	return qhi_process_hash(hash, (void *) &ctxt, php_qh_save_to_stream_func);
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
	qh_process_flags(options, flags);

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

char *qh_inthash_save_to_string(uint32_t *string_len, php_qh_inthash_obj *obj)
{
	qhi                           *hash = obj->hash;
	php_qh_save_to_string_context  ctxt;

	ctxt.string = NULL;
	ctxt.string_len = 0;

	qhi_process_hash(hash, (void *) &ctxt, php_qh_save_to_string_func);
	*string_len = ctxt.string_len;
	return ctxt.string;
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
