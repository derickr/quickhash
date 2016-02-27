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
} qh_intset_it;

/* iterator handlers */
static void qh_intset_it_dtor(zend_object_iterator *iter TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;

	qhi_iterator_deinit(&iterator->iterator);
	zval_ptr_dtor(&iterator->intern.data);

	zval_ptr_dtor(iterator->current_value);
	efree(iterator->current_value);
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
	qhi           *hash = Z_QH_INTSET_OBJ_P(&iterator->intern.data)->hash;

	ret = qhi_iterator_forward(&iterator->iterator) ? SUCCESS : FAILURE;
	switch (hash->value_type) {
		case QHI_VALUE_TYPE_INT:
			ZVAL_LONG(iterator->current_value, iterator->iterator.value.i);
			break;
		case QHI_VALUE_TYPE_STRING:
			if (Z_TYPE_P(iterator->current_value) == IS_STRING) {
				zval_dtor(iterator->current_value);
			}
			ZVAL_STRING(iterator->current_value, iterator->iterator.value.s);
			break;
	}
	return ret;
}

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

static void qh_intset_it_move_forward(zend_object_iterator *iter TSRMLS_DC)
{
}

static void qh_intset_it_rewind(zend_object_iterator *iter TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;
	qhi_iterator_init(&iterator->iterator, Z_QH_INTSET_OBJ_P(&iterator->intern.data)->hash);
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

	if (by_ref) {
		zend_error(E_ERROR, "An iterator cannot be used with foreach by reference");
	}

    zend_iterator_init(&iterator->intern);
    ZVAL_COPY(&iterator->intern.data, object);
	iterator->intern.funcs = &qh_intset_it_funcs;

    iterator->current_value = emalloc(sizeof(zval));
	ZVAL_NULL(iterator->current_value);

	return &iterator->intern;
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

	if (by_ref) {
		zend_error(E_ERROR, "An iterator cannot be used with foreach by reference");
	}

    zend_iterator_init(&iterator->intern);
	ZVAL_COPY(&iterator->intern.data, object);
	iterator->intern.funcs = &qh_inthash_it_funcs;

	iterator->current_value = emalloc(sizeof(zval));
	ZVAL_NULL(iterator->current_value);

	return &iterator->intern;
}
