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
/* $Id: php_quickhash.h,v 1.9 2009-03-30 18:22:15 derick Exp $ */

#ifndef PHP_QUICKHASH_PRIV_H
#define PHP_QUICKHASH_PRIV_H

/* refcount macros */
#ifndef Z_ADDREF_P
#define Z_ADDREF_P(pz)                (pz)->refcount++
#endif

#ifndef Z_DELREF_P
#define Z_DELREF_P(pz)                (pz)->refcount--
#endif

#ifndef Z_SET_REFCOUNT_P
#define Z_SET_REFCOUNT_P(pz, rc)      (pz)->refcount = rc
#endif

#ifndef Z_UNSET_ISREF_P
#define Z_UNSET_ISREF_P(pz)           (pz)->is_ref = 0
#endif

/* Bitfield for hash options */
#define QH_NO_DUPLICATES          0x0001
#define QH_DO_NOT_USE_ZEND_ALLOC  0x0002

#define QH_HASHER_NO_HASH         0x0100
#define QH_HASHER_JENKINS1        0x0200
#define QH_HASHER_JENKINS2        0x0400
#define QH_HASHER_MASK            0xFF00

#if PHP_VERSION_ID < 70000
# define QH_PHP_OBJ zend_object std; qhi* hash;
# define ZEND_OBJECT_VALUE_PTR zend_object_value
# define ZEND_OBJECT_PTR void*
# define TYPE_ARG_L int
#else
# define QH_PHP_OBJ qhi* hash; zend_object std;
# define ZEND_OBJECT_VALUE_PTR zend_object*
# define ZEND_OBJECT_PTR ZEND_OBJECT_VALUE_PTR
# define TYPE_ARG_L size_t
#endif

#if PHP_VERSION_ID >= 80000
# define TSRMLS_DC
# define TSRMLS_D
# define TSRMLS_CC
# define TSRMLS_C
# define TSRMLS_FETCH()
#endif

typedef struct _php_qh_string_context {
	char     *string;
	uint32_t  string_len;
	char     *ptr;
} php_qh_string_context;

typedef struct _php_qh_stream_context {
	php_stream *stream;
} php_qh_stream_context;

zval *qh_instantiate(zend_class_entry *pce, zval *object TSRMLS_DC);
void qh_add_constants(zend_class_entry *ce TSRMLS_DC);
void qh_process_flags(qho *options, long flags);
void qh_set_memory_functions(qho *options);

typedef int (*php_qh_stream_validator)(php_stream_statbuf finfo, php_stream *stream, uint32_t *nr_of_elements, uint32_t *value_array_length);
typedef int (*php_qh_string_validator)(char *string, long length, uint32_t *nr_of_elements, uint32_t *value_array_length);

int32_t php_qh_get_size_from_string(void *context);
int php_qh_prepare_string(qhi **hash, qho *options, char *string, long length, long size, long flags, php_qh_string_validator validator, uint32_t *nr_of_elements, uint32_t *value_array_length TSRMLS_DC);
int php_qh_load_int32t_from_string_func(void *context, int32_t *buffer, uint32_t elements);
int php_qh_load_chars_from_string_func(void *context, char *buffer, uint32_t elements);
int php_qh_save_int32t_to_string_func(void *context, int32_t *buffer, uint32_t elements);
int php_qh_save_chars_to_string_func(void *context, char *buffer, uint32_t elements);

int32_t php_qh_get_size_from_stream(void *context);
int php_qh_prepare_file(qhi **hash, qho *options, php_stream *stream, long size, long flags, php_qh_stream_validator validator, uint32_t *nr_of_elements, uint32_t *value_array_length TSRMLS_DC);
int php_qh_load_int32t_from_stream_func(void *context, int32_t *buffer, uint32_t elements);
int php_qh_load_chars_from_stream_func(void *context, char *buffer, uint32_t elements);
int php_qh_save_int32t_to_stream_func(void *context, int32_t *buffer, uint32_t elements);
int php_qh_save_chars_to_stream_func(void *context, char *buffer, uint32_t elements);

#endif
