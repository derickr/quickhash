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
#if HAVE_CONFIG_H
# include <config.h>
#endif
#if HAVE_STDINT_H
# include <stdint.h>
#endif
#if HAVE_STDLIB_H
# include <stdlib.h>
#endif
#if HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#if HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#if HAVE_STRING_H
# include <string.h>
#endif

#ifndef QUICK_HASH_H
#define QUICK_HASH_H

/**
 * Hasher algorithms
 */
typedef uint32_t (*qha_t)(uint32_t key);
typedef uint32_t (*qhsa_t)(char *key);

/**
 * Union value type used for hash values and keys.
 */
typedef union _qhv {
	int32_t  i;
	char    *s;
} qhv;

/**
 * Hash buckets
 */
typedef struct _qhb {
	struct _qhb *next;
	int32_t      key;
	uint32_t     value_idx;
} qhb;

/**
 * List of hash buckets
 */
typedef struct _qhl {
	qhb *head;
	qhb *tail;
	uint32_t size;
} qhl;

/**
 * Memory allocation functions struct
 */
typedef struct _qhm {
	void *(*malloc)(size_t size);
	void *(*calloc)(size_t nmemb, size_t size);
	void (*free)(void *ptr);
	void *(*realloc)(void *ptr, size_t size);
} qhm;

/**
 * Contains options for the hashes:
 * - size: the number of hash bucket lists to create
 * - check_for_dupes: whether insertions should be checked for duplicates
 */
typedef struct _qho {
	char     key_type; // either int or string
	char     value_type; // either int or string
	uint32_t size;
	char     check_for_dupes;
	qha_t    hasher;
	qhsa_t   shasher;
	qhm      memory;
} qho;

/**
 * Contains a string list
 */
typedef struct _qhsl {
	uint32_t  count;
	uint32_t  size;
	char     *data;
} qhsl;

/**
 * Integer hash type
 */
typedef struct _qhi {
	char      key_type; // either int or string
	char      value_type; // either int or string

	// hash related
	qha_t     hasher;
	qhsa_t    shasher;
	qho      *options;

	// bucket lists
	uint32_t  bucket_count;
	qhl      *bucket_list;

	// for pre-allocating buckets
	int32_t   bucket_buffer_nr;
	uint32_t  bucket_buffer_pos;
	qhb     **bucket_buffer;

	// for storing string keys
	struct {
		uint32_t  count;
		uint32_t  size;
		char     *values;
	} keys;

	// for int values
	struct {
		uint32_t  count;
		uint32_t  size;
		int32_t  *values;
	} i;

	// for string values
	struct {
		uint32_t  count;
		uint32_t  size;
		char     *values;
	} s;

	// statistics
	uint32_t  element_count;
	uint32_t  iterator_count;
#if DEBUG
	uint32_t  collisions;
#endif
} qhi;

/**
 * Iterator type
 */
typedef struct _qhit {
	qhi      *hash;

	uint32_t  bucket_list_idx;
	qhb      *current_bucket;

	int32_t   key;
	qhv       value;
} qhit;

/**
 * Constants
 */
#define QHI_KEY_TYPE_INT      1
#define QHI_KEY_TYPE_STRING   2

#define QHI_VALUE_TYPE_INT    1
#define QHI_VALUE_TYPE_STRING 2

/**
 * Function type to be used as an utility function with qhi_process_set
 */
typedef int32_t (*qhi_buffer_get_size)(void *context);
typedef int32_t (*qhi_int32t_write_buffer_apply_func)(void *context, int32_t *buffer, uint32_t elements);
typedef int32_t (*qhi_char_write_buffer_apply_func)(void *context, char *buffer, uint32_t elements);
typedef int32_t (*qhi_int32t_read_buffer_apply_func)(void *context, int32_t *buffer, uint32_t elements);
typedef int32_t (*qhi_char_read_buffer_apply_func)(void *context, char *buffer, uint32_t elements);

uint32_t qha_jenkins1(uint32_t key);
uint32_t qha_jenkins2(uint32_t key);
uint32_t qha_no_hash(uint32_t key);


qho *qho_create(void);
void qho_free(qho *options);

uint32_t qhi_normalize_size(uint32_t size);
qhi *qhi_create(qho *options);
void qhi_free(qhi *hash);

/* sets */
int qhi_set_add(qhi *hash, qhv position);
int qhi_set_exists(qhi *hash, qhv position);
int qhi_set_delete(qhi *hash, qhv position);

int qhi_process_set(qhi *hash, void *context, qhi_int32t_write_buffer_apply_func apply_func);
qhi *qhi_obtain_set(qho *options, void *context, qhi_buffer_get_size get_size, qhi_int32t_read_buffer_apply_func int32t_apply_func);
uint32_t qhi_set_add_elements_from_buffer(qhi *hash, int32_t *buffer, uint32_t nr_of_elements);
qhi *qhi_set_load_from_file(int fd, qho *options);
int qhi_set_save_to_file(int fd, qhi *hash);

/* hash */
int qhi_hash_add(qhi *hash, qhv position, qhv value);
int qhi_hash_get(qhi *hash, qhv position, qhv *value);
int qhi_hash_update(qhi *hash, qhv position, qhv value);
int qhi_hash_set(qhi *hash, qhv position, qhv value);

int qhi_process_hash(qhi *hash, void *context, qhi_int32t_write_buffer_apply_func apply_func, qhi_char_write_buffer_apply_func);
qhi *qhi_obtain_hash(qho *options, void *context, qhi_int32t_read_buffer_apply_func int32t_apply_func, qhi_char_read_buffer_apply_func chars_apply_func);
uint32_t qhi_hash_add_elements_from_buffer(qhi *hash, int32_t *buffer, uint32_t nr_of_elements);
qhi *qhi_hash_load_from_file(int fd, qho *options);
int qhi_hash_save_to_file(int fd, qhi *hash);

/* iterators (from iterator.c) */
void qhi_iterator_init(qhit *iter, qhi *hash);
int qhi_iterator_forward(qhit *iter);
void qhi_iterator_deinit(qhit *iter);

#endif
