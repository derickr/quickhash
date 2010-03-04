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
	zval                 *object_ref; /* Used so that we can protect the object from being destroyed prematurely */
} qh_intset_it;

/* iterator handlers */
static void qh_intset_it_dtor(zend_object_iterator *iter TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;

	Z_DELREF_P(iterator->object_ref);
	qhi_iterator_deinit(&iterator->iterator);
	zval_ptr_dtor(&iterator->current_value);
	efree(iterator);
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

	ret = qhi_iterator_forward(&iterator->iterator) ? SUCCESS : FAILURE;
	ZVAL_LONG(iterator->current_value, iterator->iterator.value);

	return ret;
}

static void qh_intset_it_current_data(zend_object_iterator *iter, zval ***data TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;

	*data = &iterator->current_value;
}

static int qh_intset_it_current_key(zend_object_iterator *iter, char **str_key, uint *str_key_len, ulong *int_key TSRMLS_DC)
{
	qh_intset_it   *iterator = (qh_intset_it *)iter;

	*int_key = iterator->iterator.key;
	return HASH_KEY_IS_LONG;
}

static void qh_intset_it_move_forward(zend_object_iterator *iter TSRMLS_DC)
{
	qh_intset_it   *iterator = (qh_intset_it *)iter;
}

static void qh_intset_it_rewind(zend_object_iterator *iter TSRMLS_DC)
{
	qh_intset_it *iterator = (qh_intset_it *)iter;
	qhi           *hash = (qhi* ) iterator->intern.data;

	qhi_iterator_init(&iterator->iterator, hash);
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
	php_qh_intset_obj  *obj    = (php_qh_intset_obj *)zend_object_store_get_object(object TSRMLS_CC);

	if (by_ref) {
		zend_error(E_ERROR, "An iterator cannot be used with foreach by reference");
	}

	iterator->intern.data = (void*) obj->hash;
	iterator->intern.funcs = &qh_intset_it_funcs;
	iterator->object_ref = object;
	Z_ADDREF_P(iterator->object_ref);

	MAKE_STD_ZVAL(iterator->current_value);
	ZVAL_NULL(iterator->current_value);

	return (zend_object_iterator*)iterator;
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
	php_qh_intset_obj  *obj    = (php_qh_intset_obj *)zend_object_store_get_object(object TSRMLS_CC);

	if (by_ref) {
		zend_error(E_ERROR, "An iterator cannot be used with foreach by reference");
	}

	iterator->intern.data = (void*) obj->hash;
	iterator->intern.funcs = &qh_inthash_it_funcs;
	iterator->object_ref = object;
	Z_ADDREF_P(iterator->object_ref);

	MAKE_STD_ZVAL(iterator->current_value);
	ZVAL_NULL(iterator->current_value);

	return (zend_object_iterator*)iterator;
}
