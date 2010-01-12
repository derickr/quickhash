/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2010 The PHP Group                                |
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
/* $Id: quickhash.c,v 1.40 2009-03-30 18:36:55 derick Exp $ */

#include "php.h"
#include "zend.h"
#include "zend_API.h"
#include "qh_intset.h"

zend_class_entry *qh_ce_intset;

PHPAPI zend_class_entry *php_qh_get_intset_ce(void)
{
	return qh_ce_intset;
}

zend_object_handlers qh_object_handlers_intset;

static void qh_object_free_storage_intset(void *object TSRMLS_DC);
static zend_object_value qh_object_new_intset(zend_class_entry *class_type TSRMLS_DC);

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_construct, 0, 0, 1)
	ZEND_ARG_INFO(0, size)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_add, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_qh_intset_exists, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()


zend_function_entry qh_funcs_intset[] = {
	PHP_ME(QuickHashIntSet, __construct, arginfo_qh_intset_construct, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet, add,         arginfo_qh_intset_add,       ZEND_ACC_PUBLIC)
	PHP_ME(QuickHashIntSet, exists,      arginfo_qh_intset_exists,    ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void qh_register_class_intset(TSRMLS_D)
{
	zend_class_entry ce_intset;

	INIT_CLASS_ENTRY(ce_intset, "QuickHashIntSet", qh_funcs_intset);
	ce_intset.create_object = qh_object_new_intset;
	qh_ce_intset = zend_register_internal_class_ex(&ce_intset, NULL, NULL TSRMLS_CC);
	memcpy(&qh_object_handlers_intset, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
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
	zend_hash_copy(intern->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
	
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

	/* internal cleanup */

	zend_object_std_dtor(&intern->std TSRMLS_CC);
	efree(object);
}


PHP_METHOD(QuickHashIntSet, __construct)
{
}

PHP_METHOD(QuickHashIntSet, add)
{
}

PHP_METHOD(QuickHashIntSet, exists)
{
}
