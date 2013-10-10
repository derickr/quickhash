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

#include "php.h"
#include "zend.h"
#include "zend_API.h"
#include "qh_intset.h"
#include "qh_iterator.h"
#include "quickhash.h"
#include "zend_interfaces.h"

zend_class_entry *qh_ce_intset;

PHPAPI zend_class_entry *php_qh_get_intset_ce(void)
{
	return qh_ce_intset;
}

zend_object_handlers qh_object_handlers_intset;

static void qh_object_free_storage_intset(void *object TSRMLS_DC);
static zend_object_value qh_object_new_intset(zend_class_entry *class_type TSRMLS_DC);

/* Reflection Information Structs */
ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, size)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_get_size, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_add, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_exists, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_delete, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_load_from_file, 0, 0, 1)
	ZEND_ARG_INFO(0, filename)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_save_to_file, 0, 0, 1)
	ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_load_from_string, 0, 0, 1)
	ZEND_ARG_INFO(0, contents)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_save_to_string, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_array_access_set, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* Class methods definition */
zend_function_entry qh_funcs_intset[] = {
	PHP_ME(QuickHashIntSet, __construct,    arginfo_qh_intset_construct,        ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet, getSize,        arginfo_qh_intset_get_size,         ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet, add,            arginfo_qh_intset_add,              ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet, exists,         arginfo_qh_intset_exists,           ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet, delete,         arginfo_qh_intset_exists,           ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet, loadFromFile,   arginfo_qh_intset_load_from_file,   ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet, saveToFile,     arginfo_qh_intset_save_to_file,     ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet, loadFromString, arginfo_qh_intset_load_from_string, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet, saveToString,   arginfo_qh_intset_save_to_string,   ZEND_ACC_PUBLIC)

	/* ArrayAccess methods */
	PHP_MALIAS(QuickHashIntSet, offsetExists, exists, arginfo_qh_intset_exists, ZEND_ACC_PUBLIC)
	PHP_MALIAS(QuickHashIntSet, offsetGet,    exists, arginfo_qh_intset_exists, ZEND_ACC_PUBLIC)
	PHP_MALIAS(QuickHashIntSet, offsetSet,    add,    arginfo_qh_intset_array_access_set, ZEND_ACC_PUBLIC)
	PHP_MALIAS(QuickHashIntSet, offsetUnset,  delete, arginfo_qh_intset_delete, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void qh_register_class_intset(TSRMLS_D)
{
	zend_class_entry ce_intset;

	INIT_CLASS_ENTRY(ce_intset, "QuickHashIntSet", qh_funcs_intset);
	ce_intset.create_object = qh_object_new_intset;
	qh_ce_intset = zend_register_internal_class_ex(&ce_intset, NULL, NULL TSRMLS_CC);

	qh_ce_intset->get_iterator = qh_intset_get_iterator;
	qh_ce_intset->iterator_funcs.funcs = &qh_intset_it_funcs;

	memcpy(&qh_object_handlers_intset, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	qh_add_constants(qh_ce_intset TSRMLS_CC);

	zend_class_implements(qh_ce_intset TSRMLS_CC, 1, zend_ce_arrayaccess);
}

static inline zend_object_value qh_object_new_intset_ex(zend_class_entry *class_type, php_qh_intset_obj **ptr TSRMLS_DC)
{
	php_qh_intset_obj *intern;
	zend_object_value retval;
	zval *tmp;

	intern = emalloc(sizeof(php_qh_intset_obj));
	memset(intern, 0, sizeof(php_qh_intset_obj));
	if (ptr) {
		*ptr = intern;
	}

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
#if PHP_MINOR_VERSION > 3
	object_properties_init(&intern->std, class_type);
#else
	zend_hash_copy(intern->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
#endif
	
	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) qh_object_free_storage_intset, NULL TSRMLS_CC);
	retval.handlers = &qh_object_handlers_intset;
	
	return retval;
}

static zend_object_value qh_object_new_intset(zend_class_entry *class_type TSRMLS_DC)
{
	return qh_object_new_intset_ex(class_type, NULL TSRMLS_CC);
}

static void qh_object_free_storage_intset(void *object TSRMLS_DC)
{
	php_qh_intset_obj *intern = (php_qh_intset_obj *) object;

	if (intern->hash) {
		qho *tmp_options = intern->hash->options;

		qhi_free(intern->hash);
		qho_free(tmp_options);
	}

	zend_object_std_dtor(&intern->std TSRMLS_CC);
	efree(object);
}


static int qh_intset_initialize(php_qh_intset_obj *obj, long size, long flags TSRMLS_DC)
{
	qho *options = qho_create();

	options->size = size;
	qh_process_flags(options, flags);

	obj->hash = qhi_create(options);
	if (obj->hash == NULL) {
		qho_free(options);
		return 0;
	}
	return 1;
}

/* {{{ proto QuickHashIntSet QuickHashIntSet::__construct( int size, [ int flags] )
   Creates a new QuickHashIntSet */
PHP_METHOD(QuickHashIntSet, __construct)
{
	long size;
	long flags = 0;

	QH_SET_ERROR_HANDLING(EH_THROW, NULL);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &size, &flags) == SUCCESS) {
		if (!qh_intset_initialize(zend_object_store_get_object(getThis() TSRMLS_CC), size, flags TSRMLS_CC)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not initialize set.");
		}
	}
	QH_SET_ERROR_HANDLING(EH_NORMAL, NULL);
}
/* }}} */

/* {{{ proto int QuickHashIntSet::getSize()
   Returns the size of the set */
PHP_METHOD(QuickHashIntSet, getSize)
{
	zval              *object;
	php_qh_intset_obj *intset_obj;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, qh_ce_intset) == FAILURE) {
		RETURN_FALSE;
	}
	intset_obj = (php_qh_intset_obj *) zend_object_store_get_object(object TSRMLS_CC);
	RETURN_LONG(intset_obj->hash->element_count);
}
/* }}} */

/* {{{ proto bool QuickHashIntSet::add( int key )
   Adds an element with key key to the set */
PHP_METHOD(QuickHashIntSet, add)
{
	zval              *object;
	php_qh_intset_obj *intset_obj;
	long               key;
	long               dummy; /* Dummy long so that it can work with ArrayAccess. */

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol|l", &object, qh_ce_intset, &key, &dummy) == FAILURE) {
		RETURN_FALSE;
	}
	intset_obj = (php_qh_intset_obj *) zend_object_store_get_object(object TSRMLS_CC);
	RETURN_BOOL(qhi_set_add(intset_obj->hash, (qhv) (int32_t) key));
}
/* }}} */

/* {{{ proto bool QuickHashIntSet::exists( int key )
   Tests whether the element with key key is part of the set */
PHP_METHOD(QuickHashIntSet, exists)
{
	zval              *object;
	php_qh_intset_obj *intset_obj;
	long               key;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &object, qh_ce_intset, &key) == FAILURE) {
		RETURN_FALSE;
	}
	intset_obj = (php_qh_intset_obj *) zend_object_store_get_object(object TSRMLS_CC);
	RETURN_BOOL(qhi_set_exists(intset_obj->hash, (qhv) (int32_t) key));
}
/* }}} */

/* {{{ proto bool QuickHashIntSet::delete( int key )
   Deletes an entry with the key from the set */
PHP_METHOD(QuickHashIntSet, delete)
{
	zval              *object;
	php_qh_intset_obj *intset_obj;
	long               key;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &object, qh_ce_intset, &key) == FAILURE) {
		RETURN_FALSE;
	}
	intset_obj = (php_qh_intset_obj *) zend_object_store_get_object(object TSRMLS_CC);
	RETURN_BOOL(qhi_set_delete(intset_obj->hash, (qhv) (int32_t) key));
}
/* }}} */

/* Validates whether the stream is in the correct format */
static int qh_intset_stream_validator(php_stream_statbuf finfo, php_stream *stream, uint32_t *nr_of_elements, uint32_t *value_array_length)
{
	// if the filesize is not an increment of req_count * sizeof(int32_t), abort
	if (finfo.sb.st_size % sizeof(int32_t) != 0) {
		return 0;
	}
	*nr_of_elements = finfo.sb.st_size / sizeof(int32_t);
	return 1;
}

static uint32_t qh_intset_initialize_from_file(php_qh_intset_obj *obj, php_stream *stream, long size, long flags TSRMLS_DC)
{
	uint32_t   nr_of_elements;
	qho       *options = qho_create();
	php_qh_stream_context ctxt;

	if (!php_qh_prepare_file(&obj->hash, options, stream, size, flags, qh_intset_stream_validator, &nr_of_elements, NULL TSRMLS_CC)) {
		qho_free(options);
		return 0;
	}

	ctxt.stream = stream;

	obj->hash = qhi_obtain_set(options, (void*) &ctxt, php_qh_get_size_from_stream, php_qh_load_int32t_from_stream_func);
	return obj->hash->element_count;
}

/* {{{ proto QuickHashIntSet QuickHashIntSet::loadFromFile( string filename [, int size [, int flags ]] )
   Creates a QuickHashIntSet from data in file filename */
PHP_METHOD(QuickHashIntSet, loadFromFile)
{
	char *filename;
	int   filename_len;
	long  size = 0, flags = 0;
	php_stream *stream;

	QH_SET_ERROR_HANDLING(EH_THROW, NULL);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &filename, &filename_len, &size, &flags) == FAILURE) {
		return;
	}

	if (!filename_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Filename cannot be empty");
	}

	qh_instantiate(qh_ce_intset, return_value TSRMLS_CC);
	stream = php_stream_open_wrapper(filename, "r", IGNORE_PATH | REPORT_ERRORS, NULL);
	if (stream) {
		qh_intset_initialize_from_file(zend_object_store_get_object(return_value TSRMLS_CC), stream, size, flags TSRMLS_CC);
		php_stream_close(stream);
	}
	QH_SET_ERROR_HANDLING(EH_NORMAL, NULL);
}
/* }}} */

int qh_intset_save_to_file(php_stream *stream, php_qh_intset_obj *obj)
{
	qhi                   *hash = obj->hash;
	php_qh_stream_context  ctxt;

	ctxt.stream = stream;

	return qhi_process_set(hash, (void *) &ctxt, php_qh_save_int32t_to_stream_func);
}

/* {{{ proto void QuickHashIntSet::saveToFile( string filename )
   Saves the hash to a file */
PHP_METHOD(QuickHashIntSet, saveToFile)
{
	char *filename;
	int   filename_len;
	zval              *object;
	php_qh_intset_obj *intset_obj;
	php_stream *stream;

	QH_SET_ERROR_HANDLING(EH_THROW, NULL);
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &object, qh_ce_intset, &filename, &filename_len) == FAILURE) {
		return;
	}

	if (!filename_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Filename cannot be empty");
	}

	intset_obj = (php_qh_intset_obj *) zend_object_store_get_object(object TSRMLS_CC);
	stream = php_stream_open_wrapper(filename, "w", IGNORE_PATH | REPORT_ERRORS, NULL);

	if (stream) {
		qh_intset_save_to_file(stream, intset_obj);
		php_stream_close(stream);
	}
	QH_SET_ERROR_HANDLING(EH_NORMAL, NULL);
}
/* }}} */

/* Validates whether the string is in the correct format */
static int qh_intset_string_validator(char *string, long length, uint32_t *nr_of_elements, uint32_t *value_array_length)
{
	// if the length is not an increment of req_count * sizeof(int32_t), abort
	if (length % sizeof(int32_t) != 0) {
		return 0;
	}
	*nr_of_elements = length / sizeof(int32_t);
	return 1;
}

static uint32_t qh_intset_initialize_from_string(php_qh_intset_obj *obj, char *contents, long length, long size, long flags TSRMLS_DC)
{
	uint32_t  nr_of_elements;
	qho      *options = qho_create();
	php_qh_string_context ctxt;

	if (!php_qh_prepare_string(&obj->hash, options, contents, length, size, flags, qh_intset_string_validator, &nr_of_elements, NULL TSRMLS_CC)) {
		qho_free(options);
		return 0;
	}

	ctxt.string = contents;
	ctxt.string_len = length;
	ctxt.ptr = ctxt.string;

	obj->hash = qhi_obtain_set(options, (void*) &ctxt, php_qh_get_size_from_string, php_qh_load_int32t_from_string_func);
	return obj->hash->element_count;
}

/* {{{ proto QuickHashIntSet QuickHashIntSet::loadFromString( string contents [, int size [, int flags ]] )
   Creates a QuickHashIntSet from data in a string */
PHP_METHOD(QuickHashIntSet, loadFromString)
{
	char    *contents;
	int      contents_len;
	long     size = 0, flags = 0;

	QH_SET_ERROR_HANDLING(EH_THROW, NULL);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &contents, &contents_len, &size, &flags) == FAILURE) {
		return;
	}

	qh_instantiate(qh_ce_intset, return_value TSRMLS_CC);
	qh_intset_initialize_from_string(zend_object_store_get_object(return_value TSRMLS_CC), contents, contents_len, size, flags TSRMLS_CC);
	QH_SET_ERROR_HANDLING(EH_NORMAL, NULL);
}
/* }}} */

char *qh_intset_save_to_string(uint32_t *string_len, php_qh_intset_obj *obj)
{
	qhi                   *hash = obj->hash;
	php_qh_string_context  ctxt;

	ctxt.string = NULL;
	ctxt.string_len = 0;

	qhi_process_set(hash, (void *) &ctxt, php_qh_save_int32t_to_string_func);
	*string_len = ctxt.string_len;
	return ctxt.string;
}

/* {{{ proto string QuickHashIntSet::saveToString()
   Returns the hash as a string */
PHP_METHOD(QuickHashIntSet, saveToString)
{
	zval              *object;
	php_qh_intset_obj *intset_obj;
	char              *string;
	uint32_t           string_len;

	QH_SET_ERROR_HANDLING(EH_THROW, NULL);
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, qh_ce_intset) == FAILURE) {
		return;
	}

	intset_obj = (php_qh_intset_obj *) zend_object_store_get_object(object TSRMLS_CC);

	string = qh_intset_save_to_string(&string_len, intset_obj);
	QH_SET_ERROR_HANDLING(EH_NORMAL, NULL);
	RETURN_STRINGL(string, string_len, 0);
}
/* }}} */
