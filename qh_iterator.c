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
#include "qh_inthash.h"
#include "qh_intset.h"
#include "qh_iterator.h"
#include "quickhash.h"

/* define an overloaded iterator structure */
typedef struct {
	zend_object_iterator  intern;
	zval                 *current_value;
	qhit                  iterator;
#if PHP_VERSION_ID < 70000
	zval                 *object_ref; /* Used so that we can protect the object from being destroyed prematurely */
#endif
} qh_intset_it;

/* iterator handlers */
static void qh_intset_it_dtor(zend_object_iterator *iter TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;
#if PHP_VERSION_ID < 70000
	Z_DELREF_P(iterator->object_ref);
#endif
	qhi_iterator_deinit(&iterator->iterator);
#if PHP_VERSION_ID < 70000
	qhi_iterator_deinit(&iterator->iterator);
	zval_ptr_dtor(&iterator->current_value);
	efree(iterator);
#else
	zval_ptr_dtor(&iterator->intern.data);
	zval_ptr_dtor(iterator->current_value);
	efree(iterator->current_value);
#endif
}

static int qh_intset_it_has_more(zend_object_iterator *iter TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;
	int            ret;

	ret = qhi_iterator_forward(&iterator->iterator) ? SUCCESS : FAILURE;
	ZVAL_LONG(iterator->current_value, iterator->iterator.key);

	return ret;
}

static int qh_inthash_it_has_more(zend_object_iterator *iter TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;
	int            ret;
#if PHP_VERSION_ID < 70000
	qhi           *hash = (qhi* ) iterator->intern.data;
#else
	qhi           *hash = Z_QH_INTSET_OBJ_P(&iterator->intern.data)->hash;
#endif

	ret = qhi_iterator_forward(&iterator->iterator) ? SUCCESS : FAILURE;
	switch (hash->value_type) {
		case QHI_VALUE_TYPE_INT:
			ZVAL_LONG(iterator->current_value, iterator->iterator.value.i);
			break;
		case QHI_VALUE_TYPE_STRING:
			if (Z_TYPE_P(iterator->current_value) == IS_STRING) {
				zval_dtor(iterator->current_value);
			}
#if PHP_VERSION_ID < 70000
			ZVAL_STRING(iterator->current_value, iterator->iterator.value.s, 1);
#else
			ZVAL_STRING(iterator->current_value, iterator->iterator.value.s);
#endif
			break;
	}

	return ret;
}

#if PHP_VERSION_ID < 70000
static void qh_intset_it_current_data(zend_object_iterator *iter, zval ***data TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;

	*data = &iterator->current_value;
}

static int qh_intset_it_current_key(zend_object_iterator *iter, char **str_key, uint *str_key_len, ulong *int_key TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;
	qhi          *hash = (qhi* ) iterator->intern.data;

	if (hash->key_type == QHI_KEY_TYPE_STRING) {
		*str_key = estrndup(hash->keys.values + iterator->iterator.key, strlen(hash->keys.values + iterator->iterator.key));
		*str_key_len = strlen(*str_key) + 1;
		return HASH_KEY_IS_STRING;
	} else {
		*int_key = iterator->iterator.key;
		return HASH_KEY_IS_LONG;
	}
}
#else
static zval* qh_intset_it_current_data(zend_object_iterator *iter)
{
	return ((qh_intset_it *)iter)->current_value;
}

static void qh_intset_it_current_key(zend_object_iterator *iter, zval *key)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;
	qhi          *hash = Z_QH_INTSET_OBJ_P(&iterator->intern.data)->hash;

	if (hash->key_type == QHI_KEY_TYPE_STRING) {
        ZVAL_STRINGL(key, hash->keys.values + iterator->iterator.key, strlen(hash->keys.values + iterator->iterator.key));
	} else {
        ZVAL_LONG(key, iterator->iterator.key);
	}
}
#endif

static void qh_intset_it_move_forward(zend_object_iterator *iter TSRMLS_DC)
{
}

static void qh_intset_it_rewind(zend_object_iterator *iter TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;
#if PHP_VERSION_ID < 70000
	qhi_iterator_init(&iterator->iterator, (qhi*)iterator->intern.data);
#else
	qhi_iterator_init(&iterator->iterator, Z_QH_INTSET_OBJ_P(&iterator->intern.data)->hash);
#endif
}

/* iterator handler table for sets */
zend_object_iterator_funcs qh_intset_it_funcs = {
	qh_intset_it_dtor,
	qh_intset_it_has_more,
	qh_intset_it_current_data,
	qh_intset_it_current_key,
	qh_intset_it_move_forward,
	qh_intset_it_rewind,
	NULL
};

zend_object_iterator *qh_intset_get_iterator(zend_class_entry *ce, zval *object, int by_ref TSRMLS_DC)
{
	qh_intset_it  *iterator = emalloc(sizeof(qh_intset_it));
#if PHP_VERSION_ID < 70000
	php_qh_intset_obj  *obj    = (php_qh_intset_obj *)zend_object_store_get_object(object TSRMLS_CC);
#endif

	if (by_ref) {
		zend_error(E_ERROR, "An iterator cannot be used with foreach by reference");
	}
#if PHP_VERSION_ID < 70000
	iterator->intern.data = (void*) obj->hash;
#else
	zend_iterator_init(&iterator->intern);
	ZVAL_COPY(&iterator->intern.data, object);
#endif
	iterator->intern.funcs = &qh_intset_it_funcs;
#if PHP_VERSION_ID < 70000
	iterator->object_ref = object;
	Z_ADDREF_P(iterator->object_ref);

	MAKE_STD_ZVAL(iterator->current_value);
#else
	iterator->current_value = emalloc(sizeof(zval));
#endif
	ZVAL_NULL(iterator->current_value);
#if PHP_VERSION_ID < 70000
	return (zend_object_iterator*)iterator;
#else
	return &iterator->intern;
#endif
}

/* iterator handler table for hashes */
zend_object_iterator_funcs qh_inthash_it_funcs = {
	qh_intset_it_dtor,
	qh_inthash_it_has_more,
	qh_intset_it_current_data,
	qh_intset_it_current_key,
	qh_intset_it_move_forward,
	qh_intset_it_rewind,
	NULL
};

zend_object_iterator *qh_inthash_get_iterator(zend_class_entry *ce, zval *object, int by_ref TSRMLS_DC)
{
	qh_intset_it  *iterator = emalloc(sizeof(qh_intset_it));
#if PHP_VERSION_ID < 70000
	php_qh_intset_obj  *obj    = (php_qh_intset_obj *)zend_object_store_get_object(object TSRMLS_CC);
#endif

	if (by_ref) {
		zend_error(E_ERROR, "An iterator cannot be used with foreach by reference");
	}

#if PHP_VERSION_ID < 70000
	iterator->intern.data = (void*) obj->hash;
#else
	zend_iterator_init(&iterator->intern);
	ZVAL_COPY(&iterator->intern.data, object);
#endif
	iterator->intern.funcs = &qh_inthash_it_funcs;
#if PHP_VERSION_ID < 70000
	iterator->object_ref = object;
	Z_ADDREF_P(iterator->object_ref);

	MAKE_STD_ZVAL(iterator->current_value);
#else
	iterator->current_value = emalloc(sizeof(zval));
#endif
	ZVAL_NULL(iterator->current_value);
#if PHP_VERSION_ID < 70000
	return (zend_object_iterator*)iterator;
#else
	return &iterator->intern;
#endif
}
