/*
   +----------------------------------------------------------------------+
   | PHP                                                                  |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2011 StumbleUpon Inc.                             |
   | Copyright (c) 2012-2022 Derick Rethans                               |
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

#ifndef PHP_QUICKHASH_ITERATOR_H
#define PHP_QUICKHASH_ITERATOR_H

extern zend_object_iterator_funcs qh_intset_it_funcs;
zend_object_iterator *qh_intset_get_iterator(zend_class_entry *ce, zval *object, int by_ref TSRMLS_DC);

extern zend_object_iterator_funcs qh_inthash_it_funcs;
zend_object_iterator *qh_inthash_get_iterator(zend_class_entry *ce, zval *object, int by_ref TSRMLS_DC);

#endif
