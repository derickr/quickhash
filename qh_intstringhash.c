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
/* $Id: qh_intstringhash.c 598 2010-03-20 00:06:04Z derick $ */

#include "php.h"
#include "zend.h"
#include "zend_API.h"
#include "qh_intset.h"
#include "qh_inthash.h"
#include "qh_intstringhash.h"
#include "qh_iterator.h"
#include "quickhash.h"
#include "zend_interfaces.h"

zend_class_entry *qh_ce_intstringhash;

PHPAPI zend_class_entry *php_qh_get_intstringhash_ce(void)
{
	return qh_ce_intstringhash;
}

zend_object_handlers qh_object_handlers_intstringhash;

static void qh_object_free_storage_intstringhash(void *object TSRMLS_DC);
static zend_object_value qh_object_new_intstringhash(zend_class_entry *class_type TSRMLS_DC);

/* Reflection Information Structs */
ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, size)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_get_size, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_add, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_exists, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_get, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_set, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_update, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_delete, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_load_from_file, 0, 0, 1)
	ZEND_ARG_INFO(0, filename)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_save_to_file, 0, 0, 1)
	ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_load_from_string, 0, 0, 1)
	ZEND_ARG_INFO(0, contents)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intstringhash_save_to_string, 0, 0, 0)
ZEND_END_ARG_INFO()

/* Class methods definition */
zend_function_entry qh_funcs_intstringhash[] = {
	PHP_ME(QuickHashIntStringHash, __construct,    arginfo_qh_intstringhash_construct,        ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet,        getSize,        arginfo_qh_intstringhash_get_size,         ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntStringHash, add,            arginfo_qh_intstringhash_add,              ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet,        exists,         arginfo_qh_intstringhash_exists,           ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntHash,       get,            arginfo_qh_intstringhash_get,              ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntStringHash, set,            arginfo_qh_intstringhash_set,              ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntStringHash, update,         arginfo_qh_intstringhash_update,           ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet,        delete,         arginfo_qh_intstringhash_exists,           ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntStringHash, loadFromFile,   arginfo_qh_intstringhash_load_from_file,   ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntStringHash, saveToFile,     arginfo_qh_intstringhash_save_to_file,     ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntStringHash, loadFromString, arginfo_qh_intstringhash_load_from_string, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntStringHash, saveToString,   arginfo_qh_intstringhash_save_to_string,   ZEND_ACC_PUBLIC)

	/* ArrayAccess methods */
	PHP_MALIAS(QuickHashIntSet,        offsetExists, exists, arginfo_qh_intstringhash_exists, ZEND_ACC_PUBLIC)
	PHP_MALIAS(QuickHashIntHash,       offsetGet,    get,    arginfo_qh_intstringhash_get,    ZEND_ACC_PUBLIC)
	PHP_MALIAS(QuickHashIntStringHash, offsetSet,    set,    arginfo_qh_intstringhash_set,    ZEND_ACC_PUBLIC)
	PHP_MALIAS(QuickHashIntSet,        offsetUnset,  delete, arginfo_qh_intstringhash_delete, ZEND_ACC_PUBLIC)

	{NULL, NULL, NULL}
};

void qh_register_class_intstringhash(TSRMLS_D)
{
	zend_class_entry ce_intstringhash;

	INIT_CLASS_ENTRY(ce_intstringhash, "QuickHashIntStringHash", qh_funcs_intstringhash);
	ce_intstringhash.create_object = qh_object_new_intstringhash;
	qh_ce_intstringhash = zend_register_internal_class_ex(&ce_intstringhash, php_qh_get_inthash_ce(), NULL TSRMLS_CC);

	qh_ce_intstringhash->get_iterator = qh_inthash_get_iterator;
	qh_ce_intstringhash->iterator_funcs.funcs = &qh_inthash_it_funcs;

	memcpy(&qh_object_handlers_intstringhash, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	qh_add_constants(qh_ce_intstringhash TSRMLS_CC);

	zend_class_implements(qh_ce_intstringhash TSRMLS_CC, 1, zend_ce_arrayaccess);
}

static inline zend_object_value qh_object_new_intstringhash_ex(zend_class_entry *class_type, php_qh_intstringhash_obj **ptr TSRMLS_DC)
{
	php_qh_intstringhash_obj *intern;
	zend_object_value retval;
	zval *tmp;

	intern = emalloc(sizeof(php_qh_intstringhash_obj));
	memset(intern, 0, sizeof(php_qh_intstringhash_obj));
	if (ptr) {
		*ptr = intern;
	}

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
#if PHP_MINOR_VERSION > 3
	object_properties_init(&intern->std, class_type);
#else
	zend_hash_copy(intern->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
#endif
	
	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) qh_object_free_storage_intstringhash, NULL TSRMLS_CC);
	retval.handlers = &qh_object_handlers_intstringhash;
	
	return retval;
}

static zend_object_value qh_object_new_intstringhash(zend_class_entry *class_type TSRMLS_DC)
{
	return qh_object_new_intstringhash_ex(class_type, NULL TSRMLS_CC);
}

static void qh_object_free_storage_intstringhash(void *object TSRMLS_DC)
{
	php_qh_intstringhash_obj *intern = (php_qh_intstringhash_obj *) object;

	if (intern->hash) {
		qho *tmp_options = intern->hash->options;

		qhi_free(intern->hash);
		qho_free(tmp_options);
	}

	zend_object_std_dtor(&intern->std TSRMLS_CC);
	efree(object);
}


static int qh_intstringhash_initialize(php_qh_intset_obj *obj, long size, long flags TSRMLS_DC)
{
	qho *options = qho_create();

	options->size = size;
	options->value_type = QHI_VALUE_TYPE_STRING;
	qh_process_flags(options, flags);

	obj->hash = qhi_create(options);
	if (obj->hash == NULL) {
		qho_free(options);
		return 0;
	}
	return 1;
}

/* {{{ proto QuickHashIntStringHash QuickHashIntStringHash::__construct( int size, [ int flags] )
   Creates a new QuickHashIntStringHash */
PHP_METHOD(QuickHashIntStringHash, __construct)
{
	long size;
	long flags = 0;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &size, &flags) == SUCCESS) {
		if (!qh_intstringhash_initialize(zend_object_store_get_object(getThis() TSRMLS_CC), size, flags TSRMLS_CC)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not initialize hash.");
		}
	}
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
}
/* }}} */

/* {{{ proto bool QuickHashIntStringHash::add( int key , string value )
   Adds an element with key key and value value to the hash */
PHP_METHOD(QuickHashIntStringHash, add)
{
	zval              *object;
	php_qh_intstringhash_obj *intstringhash_obj;
	long               key;
	char              *value;
	long               value_len;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ols", &object, qh_ce_intstringhash, &key, &value, &value_len) == FAILURE) {
		RETURN_FALSE;
	}
	intstringhash_obj = (php_qh_intstringhash_obj *) zend_object_store_get_object(object TSRMLS_CC);
	RETURN_BOOL(qhi_hash_add(intstringhash_obj->hash, (qhv) (int32_t) key, (qhv) value));
}
/* }}} */

/* {{{ proto int QuickHashIntStringHash::set( int key, string value )
   Updates the value of an element if it exists, or otherwise adds a new element */
PHP_METHOD(QuickHashIntStringHash, set)
{
	zval               *object;
	php_qh_intstringhash_obj *intstringhash_obj;
	long                key;
	char              *value;
	long               value_len;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ols", &object, qh_ce_intstringhash, &key, &value, &value_len) == FAILURE) {
		RETURN_FALSE;
	}
	intstringhash_obj = (php_qh_intstringhash_obj *) zend_object_store_get_object(object TSRMLS_CC);
	RETURN_LONG(qhi_hash_set(intstringhash_obj->hash, (qhv) (int32_t) key, (qhv) value));
}
/* }}} */

/* {{{ proto bool QuickHashIntStringHash::update( int key, string value )
   Updates the value of an element for key key */
PHP_METHOD(QuickHashIntStringHash, update)
{
	zval              *object;
	php_qh_intstringhash_obj *intstringhash_obj;
	long                key;
	char              *value;
	long               value_len;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ols", &object, qh_ce_intstringhash, &key, &value, &value_len) == FAILURE) {
		RETURN_FALSE;
	}
	intstringhash_obj = (php_qh_intstringhash_obj *) zend_object_store_get_object(object TSRMLS_CC);
	RETURN_BOOL(qhi_hash_update(intstringhash_obj->hash, (qhv) (int32_t) key, (qhv) value));
}
/* }}} */

/* Validates whether the stream is in the correct format */
static int qh_intstringhash_stream_validator(php_stream_statbuf finfo, php_stream *stream, uint32_t *nr_of_elements, uint32_t *value_array_length)
{
	char key_buffer[4];
	uint32_t hash_size;
	uint32_t string_store_size;

	if (php_stream_read(stream, key_buffer, 4) != 4) {
		return 0;
	}
	if (key_buffer[0] != 'Q' || key_buffer[1] != 'H' || key_buffer[2] != 0x12) {
		return 0;
	}
	if (php_stream_read(stream, (char*) &hash_size, sizeof(uint32_t)) != sizeof(uint32_t)) {
		return 0;
	}
	if (php_stream_read(stream, (char*) &string_store_size, sizeof(uint32_t)) != sizeof(uint32_t)) {
		return 0;
	}

	// if the filesize is not 12 + size * 8 + string_store_size, abort
	if (finfo.sb.st_size != ((3 * sizeof(int32_t)) + (2 * sizeof(int32_t) * hash_size) + (string_store_size))) {
		return 0;
	}
	*nr_of_elements = hash_size;

	php_stream_rewind(stream);
	return 1;
}

static uint32_t qh_intstringhash_initialize_from_file(php_qh_intstringhash_obj *obj, php_stream *stream, long size, long flags TSRMLS_DC)
{
	uint32_t  nr_of_elements;
	qho      *options = qho_create();
	php_qh_stream_context ctxt;

	if (!php_qh_prepare_file(&obj->hash, options, stream, size, flags, qh_intstringhash_stream_validator, &nr_of_elements, NULL TSRMLS_CC)) {
		qho_free(options);
		return 0;
	}

	ctxt.stream = stream;

	obj->hash = qhi_obtain_hash(options, (void*) &ctxt, php_qh_load_int32t_from_stream_func, php_qh_load_chars_from_stream_func);
	return obj->hash->element_count;
}

/* {{{ proto QuickHashIntStringHash QuickHashIntStringHash::loadFromFile( string filename [, int size [, int flags ]] )
   Creates a QuickHashIntStringHash from data in file filename */
PHP_METHOD(QuickHashIntStringHash, loadFromFile)
{
	char *filename;
	int   filename_len;
	long  size = 0, flags = 0;
	php_stream *stream;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &filename, &filename_len, &size, &flags) == FAILURE) {
		return;
	}

	if (!filename_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Filename cannot be empty");
	}

	qh_instantiate(qh_ce_intstringhash, return_value TSRMLS_CC);
	stream = php_stream_open_wrapper(filename, "r", IGNORE_PATH | REPORT_ERRORS, NULL);
	if (stream) {
		qh_intstringhash_initialize_from_file(zend_object_store_get_object(return_value TSRMLS_CC), stream, size, flags TSRMLS_CC);
		php_stream_close(stream);
	}
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
}
/* }}} */

int qh_intstringhash_save_to_file(php_stream *stream, php_qh_intstringhash_obj *obj)
{
	qhi                   *hash = obj->hash;
	php_qh_stream_context  ctxt;

	ctxt.stream = stream;

	return qhi_process_hash(hash, (void *) &ctxt, php_qh_save_int32t_to_stream_func, php_qh_save_chars_to_stream_func);
}

/* {{{ proto void QuickHashIntStringHash::saveToFile( string filename )
   Saves the hash to a file */
PHP_METHOD(QuickHashIntStringHash, saveToFile)
{
	char *filename;
	int   filename_len;
	zval              *object;
	php_qh_intstringhash_obj *intstringhash_obj;
	php_stream *stream;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &object, qh_ce_intstringhash, &filename, &filename_len) == FAILURE) {
		return;
	}

	if (!filename_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Filename cannot be empty");
	}

	intstringhash_obj = (php_qh_intstringhash_obj *) zend_object_store_get_object(object TSRMLS_CC);
	stream = php_stream_open_wrapper(filename, "w", IGNORE_PATH | REPORT_ERRORS, NULL);

	if (stream) {
		qh_intstringhash_save_to_file(stream, intstringhash_obj);
		php_stream_close(stream);
	}
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
}
/* }}} */

/* Validates whether the string is in the correct format */
static int qh_intstringhash_string_validator(char *string, long length, uint32_t *nr_of_elements, uint32_t *value_array_length)
{
	uint32_t *int_buffer = (uint32_t*) string;
	uint32_t  hash_size;
	uint32_t  string_store_size;

	if (string[0] != 'Q' || string[1] != 'H' || string[2] != 0x12) {
		return 0;
	}
	hash_size = int_buffer[1];
	string_store_size = int_buffer[2];

	// if the filesize is not 12 + size * 8 + string_store_size, abort
	if (length != ((3 * sizeof(int32_t)) + (2 * sizeof(int32_t) * hash_size) + (string_store_size))) {
		return 0;
	}
	*nr_of_elements = hash_size;

	return 1;
}

static uint32_t qh_intstringhash_initialize_from_string(php_qh_intstringhash_obj *obj, char *contents, long length, long size, long flags TSRMLS_DC)
{
	uint32_t  nr_of_elements;
	qho      *options = qho_create();
	php_qh_string_context ctxt;

	if (!php_qh_prepare_string(&obj->hash, options, contents, length, size, flags, qh_intstringhash_string_validator, &nr_of_elements, NULL TSRMLS_CC)) {
		qho_free(options);
		return 0;
	}

	ctxt.string = contents;
	ctxt.string_len = length;
	ctxt.ptr = ctxt.string;

	obj->hash = qhi_obtain_hash(options, (void*) &ctxt, php_qh_load_int32t_from_string_func, php_qh_load_chars_from_string_func);
	return obj->hash->element_count;
}

/* {{{ proto QuickHashIntStringHash QuickHashIntStringHash::loadFromString( string contents [, int size [, int flags ]] )
   Creates a QuickHashIntStringHash from data in a string */
PHP_METHOD(QuickHashIntStringHash, loadFromString)
{
	char    *contents;
	int      contents_len;
	long     size = 0, flags = 0;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &contents, &contents_len, &size, &flags) == FAILURE) {
		return;
	}

	qh_instantiate(qh_ce_intstringhash, return_value TSRMLS_CC);
	qh_intstringhash_initialize_from_string(zend_object_store_get_object(return_value TSRMLS_CC), contents, contents_len, size, flags TSRMLS_CC);
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
}
/* }}} */

char *qh_intstringhash_save_to_string(uint32_t *string_len, php_qh_intstringhash_obj *obj)
{
	qhi                   *hash = obj->hash;
	php_qh_string_context  ctxt;

	ctxt.string = NULL;
	ctxt.string_len = 0;

	qhi_process_hash(hash, (void *) &ctxt, php_qh_save_int32t_to_string_func, php_qh_save_chars_to_string_func);
	*string_len = ctxt.string_len;
	return ctxt.string;
}

/* {{{ proto string QuickHashIntStringHash::saveToString()
   Returns the hash as a string */
PHP_METHOD(QuickHashIntStringHash, saveToString)
{
	zval              *object;
	php_qh_intstringhash_obj *intstringhash_obj;
	char              *string;
	uint32_t           string_len;

	php_set_error_handling(EH_THROW, NULL TSRMLS_CC);
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &object, qh_ce_intstringhash) == FAILURE) {
		return;
	}

	intstringhash_obj = (php_qh_intstringhash_obj *) zend_object_store_get_object(object TSRMLS_CC);

	string = qh_intstringhash_save_to_string(&string_len, intstringhash_obj);
	php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
	RETURN_STRINGL(string, string_len, 0);
}
/* }}} */
