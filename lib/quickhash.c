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
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "quickhash.h"
#include "hash-algorithms.h"

/**
 * Helper struct that contains the file descriptor for use with
 * qhi_process_set or qhi_process_hash.
 */
typedef struct _fd_apply_context {
	int fd;
} fd_apply_context;

static int32_t fd_get_size_apply_func(void *context)
{
	fd_apply_context *ctxt = (fd_apply_context*) context;
	struct stat       finfo;

	if (fstat(ctxt->fd, &finfo) == 0) {
		return finfo.st_size;
	}
	return -1;
}

/**
 * Function that is used as an apply function for qhi_process_set or
 * qhi_process_hash. It is called when the buffer is full, and will write the
 * buffer's contents to the filedescriptor that is contained in the
 * fd_apply_context struct.
 *
 * Parameters:
 * - context: The context containing the filedescriptor
 * - buffer: The buffer containing the data
 * - elements: The number of elements in the buffer
 *
 * Returns:
 * - 1 if all elements could be written, or 0 if not.
 */
static int fd_write_int32t_apply_func(void *context, int32_t *buffer, uint32_t elements)
{
	fd_apply_context *ctxt = (fd_apply_context*) context;

	if (write(ctxt->fd, buffer, elements * sizeof(int32_t)) != (elements * sizeof(int32_t))) {
		return 0;
	}
	return 1;
}

/**
 * Function that is used as an apply function for qhi_process_set or
 * qhi_process_hash. It is called when the buffer is full, and will write the
 * buffer's contents to the filedescriptor that is contained in the
 * fd_apply_context struct.
 *
 * Parameters:
 * - context: The context containing the filedescriptor
 * - buffer: The buffer containing the data
 * - elements: The number of elements in the buffer
 *
 * Returns:
 * - 1 if all elements could be written, or 0 if not.
 */
static int fd_write_chars_apply_func(void *context, char *buffer, uint32_t elements)
{
	fd_apply_context *ctxt = (fd_apply_context*) context;

	if (write(ctxt->fd, buffer, elements) != elements) {
		return 0;
	}
	return 1;
}

/**
 * Function that is used qhi_obtain_hash. It is called to read data from a fd
 * into the buffer.
 *
 * Parameters:
 * - context: The context containing the filedescriptor
 * - buffer: The buffer to store the data in
 * - elements: The number of elements to read
 *
 * Returns:
 * - the number of elements (not bytes) that have been read
 */
static int fd_read_int32t_apply_func(void *context, int32_t *buffer, uint32_t elements)
{
	fd_apply_context *ctxt = (fd_apply_context*) context;

	return (read(ctxt->fd, buffer, elements * sizeof(int32_t)) / sizeof(int32_t));
}

/**
 * Function that is used qhi_obtain_hash. It is called to read data from a fd
 * into the buffer.
 *
 * Parameters:
 * - context: The context containing the filedescriptor
 * - buffer: The buffer to store the data in
 * - elements: The number of elements to read
 *
 * Returns:
 * - the number of elements (not bytes) that have been read
 */
static int fd_read_chars_apply_func(void *context, char *buffer, uint32_t elements)
{
	fd_apply_context *ctxt = (fd_apply_context*) context;

	return read(ctxt->fd, buffer, elements);
}

/**
 * Defines the number of buckets to pre-allocate
 */
#define QHB_BUFFER_PREALLOC_INC 1024

/**
 * Associates the key with a bucket.
 *
 * In case there is a string key it adds the key to the "key store", otherwise
 * it just sets the proper property of the bucket to the key. In case of a
 * string key, this function only stores the *index* into the key store.
 *
 * Params:
 * - hash: the hash that the bucket belongs to
 * - bucket: the bucket that will contain the key (or key index)
 * - key: the key itself
 */
static inline uint32_t hash_add_key(qhi *hash, qhv key)
{
	switch (hash->key_type) {
		case QHI_KEY_TYPE_INT:
			return key.i;

		case QHI_KEY_TYPE_STRING: {
			size_t str_len = strlen(key.s);
			uint32_t retval;

			if (hash->keys.count + str_len + 1 >= hash->keys.size) {
				hash->keys.values = hash->options->memory.realloc(hash->keys.values, (hash->keys.size + QHB_BUFFER_PREALLOC_INC));
				hash->keys.size += QHB_BUFFER_PREALLOC_INC;
			}
			memcpy(hash->keys.values + hash->keys.count, key.s, str_len + 1);
			retval = hash->keys.count;
			hash->keys.count = hash->keys.count + str_len + 1;
			return retval;
		} break;
	}
	return 0;
}

/**
 * Checks whether the key in the bucket, matches the one in "key".
 *
 * Params:
 * - hash: the hash that the bucket belongs to
 * - bucket: the bucket that will contain the key (or key index)
 * - key: the key itself
 *
 * Returns:
 * - 1 when the key matches, and 0 when it does not.
 */
static inline int compare_key(qhi *hash, qhb *bucket, qhv key)
{
	if (hash->key_type == QHI_KEY_TYPE_INT && bucket->key == key.i) {
		return 1;
	}
	if (hash->key_type == QHI_KEY_TYPE_STRING && memcmp(hash->keys.values + bucket->key, key.s, strlen(hash->keys.values + bucket->key)) == 0) {
		return 1;
	}
	return 0;
}

/**
 * Allocates a hash bucket.
 *
 * The algorithm allocates memory for QHB_BUFFER_PREALLOC_INC buckets at the
 * same time to avoid having too many allocs and frees.
 *
 * Returns:
 * - A newly allocated hash bucket or NULL upon allocation failure
 */
inline qhb *qhb_create(qhi *hash)
{
	void   *tmp = NULL;
	size_t  ptr_size = 0;
	size_t  bucket_size = 0;

	ptr_size = sizeof(qhb*);
	bucket_size = sizeof(qhb);

	if (hash->bucket_buffer_pos % QHB_BUFFER_PREALLOC_INC == 0) {
		hash->bucket_buffer_nr++;
		hash->bucket_buffer = hash->options->memory.realloc(hash->bucket_buffer, ptr_size * (hash->bucket_buffer_nr + 1));
		if (!hash->bucket_buffer) {
			return NULL;
		}
		hash->bucket_buffer[hash->bucket_buffer_nr] = hash->options->memory.malloc(bucket_size * QHB_BUFFER_PREALLOC_INC);
		if (!hash->bucket_buffer[hash->bucket_buffer_nr]) {
			return NULL;
		}
		hash->bucket_buffer_pos = 0;
	}

	tmp = &(hash->bucket_buffer[hash->bucket_buffer_nr][hash->bucket_buffer_pos]);

	hash->bucket_buffer_pos++;

	return tmp;
}

/**
 * Creates a new options struct and sets defaults
 *
 * Returns:
 * - An options struct with default values.
 */
qho *qho_create(void)
{
	qho *tmp = malloc(sizeof(qho));

	tmp->key_type = QHI_KEY_TYPE_INT;
	tmp->value_type = QHI_VALUE_TYPE_INT;
	tmp->size = 1024;
	tmp->check_for_dupes = 0;
	tmp->memory.malloc = malloc;
	tmp->memory.calloc = calloc;
	tmp->memory.realloc = realloc;
	tmp->memory.free = free;
	tmp->hasher = qha_jenkins2;
	tmp->shasher = qha_djb2;

	return tmp;
}

/**
 * Frees an options struct
 *
 * Parameters:
 * - options: the options struct to free
 */
void qho_free(qho *options)
{
	free(options);
}

/**
 * Normalizes the size to a power of two.
 *
 * The function also limits the size from sizes 4 to 4194304.
 *
 * Parameters:
 * - size: the number of (expected) elements in the set.
 *
 * Returns:
 * - The normalized size
 */
uint32_t qhi_normalize_size(uint32_t size)
{
	uint32_t i;

	if (size > 1<<22) {
		return 1<<22;
	}
	for (i = 22; i > 6; i--) {
		if (size > (1<< (i - 1))) {
			return 1<<i;
		}
	}
	return 64;
}

/**
 * Creates a new integer quick hash
 *
 * Parameters:
 * - options: the options to create the hash with. This structure contains at
 *   least the nr of hash buckets, and whether set additions should be checked
 *   for duplicates. See the description of qho for a full list of options.
 *   This options struct must *NOT* be freed before the hash itself, as it
 *   contains pointers to memory allocation functions.
 *
 * Returns:
 * - a pointer to the hash, or NULL if:
 *   - upon memory allocation failures.
 *   - size < 4
 */
qhi *qhi_create(qho *options)
{
	qhi      *tmp;
	uint32_t  size;

	if (options == NULL) {
		return NULL;
	}

	size = qhi_normalize_size(options->size);

	tmp = options->memory.malloc(sizeof(qhi));
	if (!tmp) {
		return NULL;
	}

	tmp->key_type = options->key_type;
	tmp->value_type = options->value_type;

	tmp->keys.count = 0;
	tmp->keys.size  = 0;
	tmp->keys.values = NULL;

	tmp->hasher = options->hasher;
	tmp->shasher = options->shasher;
	tmp->bucket_count = size;

	tmp->bucket_buffer_nr  = -1;
	tmp->bucket_buffer_pos = 0;
	tmp->bucket_buffer     = NULL;

	tmp->bucket_list = options->memory.calloc(sizeof(qhl) * size, 1);
	if (!tmp->bucket_list) {
		options->memory.free(tmp);
		return NULL;
	}

	tmp->i.count = 0;
	tmp->i.size  = 0;
	tmp->i.values = NULL;

	tmp->s.count = 0;
	tmp->s.size  = 0;
	tmp->s.values = NULL;

	tmp->options = options;

	tmp->element_count = 0;
	tmp->iterator_count = 0;
#if DEBUG
	tmp->collisions = 0;
#endif

	return tmp;
}

/**
 * Frees a quickhash
 *
 * Parameters:
 * - hash: A valid quickhash
 */
void qhi_free(qhi *hash)
{
	int32_t idx;

#if DEBUG
	printf("Elements:   %u\n", hash->element_count);
	printf("Collisions: %u\n", hash->collisions);
#endif
	for (idx = 0; idx <= hash->bucket_buffer_nr; idx++) {
		hash->options->memory.free(hash->bucket_buffer[idx]);
	}

	if (hash->bucket_buffer) {
		hash->options->memory.free(hash->bucket_buffer);
	}
	if (hash->keys.values) {
		hash->options->memory.free(hash->keys.values);
	}
	if (hash->i.values) {
		hash->options->memory.free(hash->i.values);
	}
	if (hash->s.values) {
		hash->options->memory.free(hash->s.values);
	}

	hash->options->memory.free(hash->bucket_list);
	hash->options->memory.free(hash);
}

/**
 * Obtains the hash value for the specified key
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 *
 * Returns:
 * - The hashed key
 */
inline uint32_t qhi_set_hash(qhi *hash, qhv key)
{
	uint32_t idx;

	switch (hash->key_type) {
		case QHI_KEY_TYPE_INT:
			idx = hash->hasher(key.i);
			break;

		case QHI_KEY_TYPE_STRING:
			idx = hash->shasher(key.s);
			break;

		default:
			idx = 0;
			break;
	}

	return idx & (hash->bucket_count - 1);
}

/**
 * Internal: checks whether a key exists in a specific list
 *
 * Parameters:
 * - list: the list belonging to the index of the hashed key
 * - key: the element's key
 * - value_idx: pointer to store the value_idx in
 *
 * Returns:
 * - 1 if the key exists in the list, 0 if not
 */
static int find_bucket_from_list(qhi *hash, qhl *list, qhv key, qhb **bucket)
{
	if (!list->head) {
		// there is no bucket list for this hashed key
		return 0;
	} else {
		qhb *p = (qhb*) list->head;

		// loop over the elements in this bucket list to see if the key exists
		do {
			if (compare_key(hash, p, key)) {
				if (bucket) {
					*bucket = p;
				}
				return 1;
			}
			p = p->next;
		} while(p);
	}
	return 0;
}

/**
 * Adds a new element to the hash
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 *
 * Returns:
 * - 1 if the element was added or 0 if the element couldn't be added
 */
int qhi_set_add(qhi *hash, qhv key)
{
	uint32_t idx;
	qhl     *list;
	qhb  *bucket;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	// check if we already have the key in the list if requested
	if (hash->options->check_for_dupes && find_bucket_from_list(hash, list, key, NULL)) {
		return 0;
	}

	// create new bucket
	bucket = qhb_create(hash);
	if (!bucket) {
		return 0;
	}
	bucket->key = hash_add_key(hash, key);
	bucket->next = NULL;

	// add bucket to list
	if (list->head == NULL) {
		// first bucket in list
		list->head = bucket;
		list->tail = bucket;
	} else {
		// following bucked in a list
		list->tail->next = bucket;
		list->tail = bucket;
#if DEBUG
		hash->collisions++;
#endif
	}
	hash->element_count++;
	list->size++;
	return 1;
}


/**
 * Internal: deletes an entry with the specified key from the list
 *
 * Parameters:
 * - list: the list belonging to the index of the hashed key
 * - key: the element's key
 *
 * Returns:
 * - 1 if the entry was deleted, or 0 if not
 */
static int delete_entry_from_list(qhi *hash, qhl *list, qhv key)
{
	if (!list->head) {
		// there is no bucket list for this hashed key
		return 0;
	} else {
		qhb *previous = NULL;
		qhb *current = list->head;

		// loop over the elements in this bucket list to see if the key exists,
		// also keep track of the previous one
		do {
			if (compare_key(hash, current, key)) {
				// if previous is not set, it's the first element in the list, so we just adjust head.
				if (!previous) {
					list->head = current->next;
				} else {
					previous->next = current->next;
				}
				list->size--;
				return 1;
			}
			previous = current;
			current = current->next;
		} while(current);
	}
	return 0;
}

/**
 * Deletes an element from the hash
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 *
 * Returns:
 * - 1 if the element was delete or 0 if the element couldn't be found
 */
int qhi_set_delete(qhi *hash, qhv key)
{
	uint32_t idx;
	qhl     *list;

	// if we're in iteration mode, we can't delete.
	if (hash->iterator_count > 0) {
		return 0;
	}
	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	if (!delete_entry_from_list(hash, list, key)) {
		return 0;
	}
	hash->element_count--;
	return 1;
}

/**
 * Tests whether the key exists in the set
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 *
 * Returns:
 * - 1 if the element is part of the set or 0 if the element is not part of the
 *   set
 */
int qhi_set_exists(qhi *hash, qhv key)
{
	uint32_t idx;
	qhl     *list;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	return find_bucket_from_list(hash, list, key, NULL);
}

/**
 * Adds elements contained in a buffer to set
 *
 * Parameters:
 * - hash: the set to add elements to
 * - buffer: the buffer containing the keys
 * - nr_of_elements: the number of elements in the buffer
 *
 * Returns:
 * - The number of elements that were added to the set. This number can be less
 *   than the number of elements in the buffer because de-duping might happen.
 */
uint32_t qhi_set_add_elements_from_buffer(qhi *hash, int32_t *buffer, uint32_t nr_of_elements)
{
	uint32_t i;
	uint32_t added = 0;

	for (i = 0; i < nr_of_elements; i++) {
		added += qhi_set_add(hash, (qhv) buffer[i]);
	}
	return added;
}

/**
 * Loads a set from a file pointed to by the file descriptor
 *
 * Parameters:
 * - fd: a file descriptor that is suitable for reading from
 * - options: the options to create the hash with. This structure contains at
 *   least the nr of hash buckets, and whether set additions should be checked
 *   for duplicates. See the description of qho for a full list of options.
 *
 * Returns:
 * - A new hash, or NULL upon failure
 */
qhi *qhi_obtain_set(qho *options, void *context, qhi_buffer_get_size get_size, qhi_int32t_read_buffer_apply_func int32t_apply_func)
{
	uint32_t    nr_of_elements, elements_read = 0;
	int32_t     key_buffer[1024];
	qhi        *tmp;
	int32_t     buffer_size;
	
	buffer_size = get_size(context);

	// if the filesize is not an increment of 4, abort
	if (buffer_size % 4 != 0) {
		return NULL;
	}
	nr_of_elements = buffer_size / 4;

	// override the nr of bucket lists if the size is still 0.
	options->size = qhi_normalize_size(options->size == 0 ? nr_of_elements : options->size);
#if DEBUG
	printf("Picking size: %u\n", options->size);
#endif

	// create the set
	tmp = qhi_create(options);
	if (!tmp) {
		return NULL;
	}

	// read the elements and add them to the set
	do {
		elements_read = int32t_apply_func(context, key_buffer, 1024);
		qhi_set_add_elements_from_buffer(tmp, key_buffer, elements_read);
		nr_of_elements -= elements_read;
	} while (elements_read && nr_of_elements);

	return tmp;
}

qhi *qhi_set_load_from_file(int fd, qho *options)
{
	fd_apply_context ctxt;
	ctxt.fd = fd;
	return qhi_obtain_set(options, (void*) &ctxt, fd_get_size_apply_func, fd_read_int32t_apply_func);
}

/**
 * Generic function that loops over a set and calls the apply_func when the buffer is full.
 *
 * Parameters:
 * - hash: The hash to operator on
 * - context: A structure, that is dependent on which apply_func is used. This
 *   structure is passed to the apply_func.
 * - int32t_apply_func: Whenever the buffer is full, this function is called
 *   with as parameters the context, the buffer, and the length of the buffer's
 *   contents.
 *
 * Returns:
 * - 1 of the function succeeded, and 0 if it did not.
 */
int qhi_process_set(qhi *hash, void *context, qhi_int32t_write_buffer_apply_func int32t_apply_func)
{
	uint32_t    idx;
	uint32_t    elements_in_buffer = 0;
	int32_t     key_buffer[1024];

	for (idx = 0; idx < hash->bucket_count; idx++)	{
		qhl    *list = &(hash->bucket_list[idx]);
		qhb *p = list->head;
		qhb *n;

		if (p) {
			while(p) {
				n = p->next;

				key_buffer[elements_in_buffer] = p->key;
				elements_in_buffer++;

				if (elements_in_buffer == 1024) {
					if (!int32t_apply_func(context, key_buffer, elements_in_buffer)) {
						return 0;
					}
					elements_in_buffer = 0;
				}

				p = n;
			}
		}
	}

	if (elements_in_buffer > 0) {
		if (!int32t_apply_func(context, key_buffer, elements_in_buffer)) {
			return 0;
		}
	}
	return 1;
}

/**
 * Saves a set to a file pointed to by the file descriptor
 *
 * Parameters:
 * - hash: the set to write
 * - fd: a file descriptor that is suitable for reading to
 *
 * Returns:
 * - 1 on success, and 0 on failure
 */
int qhi_set_save_to_file(int fd, qhi *hash)
{
	fd_apply_context ctxt;
	ctxt.fd = fd;
	return qhi_process_set(hash, (void *) &ctxt, fd_write_int32t_apply_func);
}

/**
 * Internal: Adds a value to the value storage and returns the index
 *
 * Parameters:
 * - hash: the hash to add the value to
 * - value: the value itself
 *
 * Returns:
 * - The index under which the value was added (which is stored in the hash)
 */
static uint32_t hash_add_value(qhi *hash, qhv value)
{
	uint32_t retval = 0;
	switch (hash->value_type) {
		case QHI_VALUE_TYPE_INT:
			if (hash->i.count == hash->i.size) {
				hash->i.values = hash->options->memory.realloc(hash->i.values, (hash->i.size + QHB_BUFFER_PREALLOC_INC) * sizeof(int32_t));
				hash->i.size += QHB_BUFFER_PREALLOC_INC;
			}
			hash->i.values[hash->i.count] = value.i;
			retval = hash->i.count;
			hash->i.count++;
			break;

		case QHI_VALUE_TYPE_STRING: {
			size_t str_len = strlen(value.s);
			if (hash->s.count + str_len + 1 >= hash->s.size) {
				hash->s.values = hash->options->memory.realloc(hash->s.values, (hash->s.size + QHB_BUFFER_PREALLOC_INC));
				hash->s.size += QHB_BUFFER_PREALLOC_INC;
			}
			memcpy(hash->s.values + hash->s.count, value.s, str_len + 1);
			retval = hash->s.count;
			hash->s.count = hash->s.count + str_len + 1;
		} break;

		default:
			/* FAIL */
			break;
	}
	return retval;
}

/**
 * Adds a new bucket with key and value to the list
 *
 * Parameters:
 * - hash: A valid quickhash
 * - list: The list that corresponds to the hashed version of the key
 * - key: The entry's key index
 * - value: The value belonging to the entry
 *
 * Returns:
 * - 1 if the element was added or 0 if the element couldn't be added
 */
static int qhi_add_entry_to_list(qhi *hash, qhl *list, uint32_t key_idx, uint32_t value_idx)
{
	qhb *bucket;

	// create new bucket
	bucket = qhb_create(hash);
	if (!bucket) {
		return 0;
	}
	bucket->key = key_idx;
	bucket->next = NULL;
	bucket->value_idx = value_idx;

	// add bucket to list
	if (list->head == NULL) {
		// first bucket in list
		list->head = bucket;
		list->tail = bucket;
	} else {
		// following bucked in a list
		list->tail->next = bucket;
		list->tail = bucket;
#if DEBUG
		hash->collisions++;
#endif
	}
	hash->element_count++;
	list->size++;
	return 1;
}

/**
 * Updates the value belonging to a bucket.
 *
 * This function updates the value that belongs to the bucket, by updating it
 * in the integer store when there is an integer values, or it will *add* a new
 * value to the store for string values. The function does not clean up holes
 * in the string store.
 *
 * Params:
 * - hash: the hash the bucket belongs to
 * - bucket: the bucket itself
 * - value: the new value
 *
 * Returns:
 * - 1 if the key was of a known type, or 0 otherwise. Only if something is
 *   really wrong, 0 should be returned.
 */
static int qhi_update_value_in_bucket(qhi *hash, qhb *bucket, qhv value)
{
	switch (hash->value_type) {
		case QHI_VALUE_TYPE_INT:
			hash->i.values[bucket->value_idx] = value.i;
			return 1;

		case QHI_VALUE_TYPE_STRING:
			bucket->value_idx = hash_add_value(hash, value);
			return 1;
			break;

		default:
			/* FAIL */
			break;
	}
	return 0;
}

/**
 * Fetches the value belonging to a bucket into "value".
 *
 * This function retrieves the integer or string value, and returns that into
 * value which has to be a pointer to an allocated qhv structure.
 *
 * Params:
 * - hash: the hash the bucket belongs to
 * - bucket: the bucket
 * - *value: a pointer to an allocated qhv structure to set the value in.
 *
 * Returns:
 * - 1 if the key was of a known type, or 0 otherwise. Only if something is
 *   really wrong, 0 should be returned.
 */
static int qhi_get_value_from_bucket(qhi *hash, qhb *bucket, qhv *value)
{
	switch (hash->value_type) {
		case QHI_VALUE_TYPE_INT:
			(*value).i = hash->i.values[bucket->value_idx];
			return 1;
			break;

		case QHI_VALUE_TYPE_STRING:
			(*value).s = &hash->s.values[bucket->value_idx];
			return 1;
			break;

		default:
			/* FAIL */
			break;
	}
	return 0;
}

/**
 * Adds a new element to the hash with a value
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 * - value: The value
 *
 * Returns:
 * - 1 if the element was added or 0 if the element couldn't be added
 */
int qhi_hash_add(qhi *hash, qhv key, qhv value)
{
	uint32_t idx;
	qhl     *list;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	// check if we already have the key in the list if requested
	if (hash->options->check_for_dupes && find_bucket_from_list(hash, list, key, NULL)) {
		return 0;
	}

	return qhi_add_entry_to_list(hash, list, hash_add_key(hash, key), hash_add_value(hash, value));
}

/**
 * Adds a new element to the hash with a value index
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 * - value_idx: The index for the associated value into the value store.
 *
 * Returns:
 * - 1 if the element was added or 0 if the element couldn't be added
 */
int qhi_hash_add_with_index(qhi *hash, qhv key, uint32_t value_index)
{
	uint32_t idx;
	qhl     *list;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	// check if we already have the key in the list if requested
	if (hash->options->check_for_dupes && find_bucket_from_list(hash, list, key, NULL)) {
		return 0;
	}

	return qhi_add_entry_to_list(hash, list, hash_add_key(hash, key), value_index);
}

/**
 * Updates a value for a key in the hash
 *
 * If there are duplicate keys in the hash, only the value of the first one
 * found will be updated.
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 * - value: The value
 *
 * Returns:
 * - 1 if the element is part of the hash and was updated or 0 if the element
 *   was not part of the hash
 */
int qhi_hash_update(qhi *hash, qhv key, qhv value)
{
	uint32_t idx;
	qhl     *list;
	qhb     *bucket;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	if (find_bucket_from_list(hash, list, key, &bucket)) {
		return qhi_update_value_in_bucket(hash, bucket, value);
	} else {
		return 0;
	}
}

/**
 * Adds a new element, or updates the value if the key already is part of the hash
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 * - value: The value
 *
 * Returns:
 * - 1 if the element is part of the hash and was updated, 2 if the element was
 *   added or 0 if an error occurred.
 */
int qhi_hash_set(qhi *hash, qhv key, qhv value)
{
	uint32_t idx;
	qhl     *list;
	qhb     *bucket;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	// check if we already have the key in the list if requested
	if (find_bucket_from_list(hash, list, key, &bucket)) {
		// update
		return qhi_update_value_in_bucket(hash, bucket, value);
	} else {
		// add
		return qhi_add_entry_to_list(hash, list, hash_add_key(hash, key), hash_add_value(hash, value)) ? 2 : 0;
	}
	return 0;
}

/**
 * Tests whether the key exists in the hash
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 *
 * Returns:
 * - 1 if the element is part of the set or 0 if the element is not part of the
 *   set
 */
int qhi_hash_get(qhi *hash, qhv key, qhv *value)
{
	uint32_t idx;
	qhl     *list;
	qhb     *bucket;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	if (find_bucket_from_list(hash, list, key, &bucket)) {
		if (value) {
			return qhi_get_value_from_bucket(hash, bucket, value);
		}
		return 1;
	} else {
		return 0;
	}
}

/**
 * Adds elements contained in a buffer to hash
 *
 * Parameters:
 * - hash: the hash to add elements to
 * - buffer: the buffer containing the keys
 * - nr_of_elements: the number of elements in the buffer
 *
 * Returns:
 * - The number of elements that were added to the set. This number can be less
 *   than the number of elements in the buffer because de-duping might happen.
 */
uint32_t qhi_hash_add_elements_from_buffer(qhi *hash, int32_t *buffer, uint32_t nr_of_elements)
{
	uint32_t i;
	uint32_t added = 0;

	for (i = 0; i < nr_of_elements; i += 2) {
		switch (hash->value_type) {
			case QHI_VALUE_TYPE_INT:
				added += qhi_hash_add(hash, (qhv) buffer[i], (qhv) buffer[i + 1]);
				break;
			case QHI_VALUE_TYPE_STRING:
				added += qhi_hash_add_with_index(hash, (qhv) buffer[i], buffer[i + 1]);
				break;
		}
	}
	return added;
}

/**
 * Creates a hash from a string containing a serialized hash
 *
 * Parameters:
 * - options: the options to create the hash with. This structure contains at
 *   least the nr of hash buckets, and whether set additions should be checked
 *   for duplicates. See the description of qho for a full list of options.
 * - context: the context that reader functions can use to store specific data
 *   in (such as a filedescription, or a PHP stream)
 * - int32t_apply_func: The function to use for reading int32s.
 * - chars_apply_func: The function to use for reading chars.
 *
 * Returns:
 * - A new hash, or NULL upon failure
 */
qhi *qhi_obtain_hash(qho *options, void *context, qhi_int32t_read_buffer_apply_func int32t_apply_func, qhi_char_read_buffer_apply_func chars_apply_func)
{
	uint32_t    nr_of_elements, elements_read = 0;
	int32_t     key_buffer[1024];
	qhi        *tmp;

	// we read the first int32s for the signature and nr of items
	if (!int32t_apply_func(context, key_buffer, 2)) {
		return NULL;
	}
	if ((key_buffer[0] & 0xffff) != 0x4851) {
		return NULL;
	}
	options->key_type = (key_buffer[0] & 0xf00000) >> 20;
	options->value_type = (key_buffer[0] & 0x0f0000) >> 16;

	nr_of_elements = key_buffer[1];

	// override the nr of bucket lists if the size is still 0.
	options->size = qhi_normalize_size(options->size == 0 ? nr_of_elements : options->size);
#if DEBUG
	printf("Picking size: %u\n", options->size);
#endif

	// read the keys if we have QHI_KEY_TYPE_STRING
	if (options->key_type == QHI_KEY_TYPE_STRING) {
		int32t_apply_func(context, key_buffer, 2);
		options->size = key_buffer[1];
#if DEBUG
		printf("Loading size: %u\n", options->size);
#endif

		// create the hash
		tmp = qhi_create(options);
		if (!tmp) {
			return NULL;
		}

		// load the keys
		tmp->keys.values = tmp->options->memory.malloc(key_buffer[0] + 1);
		tmp->keys.size = tmp->keys.count = key_buffer[0];
		chars_apply_func(context, tmp->keys.values, key_buffer[0]);
		tmp->keys.values[key_buffer[0]] = '\0';
	} else {
		// create the hash
		tmp = qhi_create(options);
		if (!tmp) {
			return NULL;
		}
	}

	// read the strings if we have QHI_VALUE_TYPE_STRING
	if (tmp->value_type == QHI_VALUE_TYPE_STRING) {
		int32t_apply_func(context, key_buffer, 1);
		tmp->s.values = tmp->options->memory.malloc(key_buffer[0] + 1);
		tmp->s.size = tmp->s.count = key_buffer[0];
		chars_apply_func(context, tmp->s.values, key_buffer[0]);
		tmp->s.values[key_buffer[0]] = '\0';
	}

	if (tmp->key_type == QHI_KEY_TYPE_STRING) {
		do {
			int32_t idx, list_elements;
			qhl *list;

			// read hash key, and nr of elements in list
			int32t_apply_func(context, key_buffer, 2);
			idx = key_buffer[0];
			list_elements = key_buffer[1];

			// read the hash elements
			do {
				int32t_apply_func(context, key_buffer, 2);
				list = &(tmp->bucket_list[idx]);
				qhi_add_entry_to_list(tmp, list, key_buffer[0], hash_add_value(tmp, (qhv) key_buffer[1]));
				elements_read++;
				list_elements--;
			} while (list_elements);
		} while (elements_read < nr_of_elements);
	} else {
		// read the elements (key and value idx) and add them to the hash
		do {
			elements_read = int32t_apply_func(context, key_buffer, 1024) / 2;
			qhi_hash_add_elements_from_buffer(tmp, key_buffer, elements_read * 2);
			nr_of_elements -= elements_read;
		} while (elements_read && nr_of_elements);
	}

	return tmp;
}

/**
 * Loads a hash from a file pointed to by the file descriptor
 *
 * Parameters:
 * - fd: a file descriptor that is suitable for reading from
 * - options: the options to create the hash with. This structure contains at
 *   least the nr of hash buckets, and whether set additions should be checked
 *   for duplicates. See the description of qho for a full list of options.
 *
 * Returns:
 * - A new hash, or NULL upon failure
 */
qhi *qhi_hash_load_from_file(int fd, qho *options)
{
	fd_apply_context ctxt;
	ctxt.fd = fd;
	return qhi_obtain_hash(options, (void*) &ctxt, fd_read_int32t_apply_func, fd_read_chars_apply_func);
}

static void add_to_buffer(qhi *hash, int32_t *key_buffer, uint32_t *elements_in_buffer, qhb *p)
{
	switch (hash->value_type) {
		case QHI_VALUE_TYPE_INT:
			key_buffer[*elements_in_buffer] = p->key;
			key_buffer[*elements_in_buffer + 1] = hash->i.values[p->value_idx];
			*elements_in_buffer += 2;
			break;

		case QHI_VALUE_TYPE_STRING:
			key_buffer[*elements_in_buffer] = p->key;
			key_buffer[*elements_in_buffer + 1] = p->value_idx;
			*elements_in_buffer += 2;
			break;

		default:
			break;
	}
}

/**
 * Generic function that loops over a hash and calls the apply_func when the buffer is full.
 *
 * Parameters:
 * - hash: The hash to operator on
 * - context: A structure, that is dependent on which apply_func is used. This
 *   structure is passed to the apply_func.
 * - apply_func: Whenever the buffer is full, this function is called with as
 *   parameters the context, the buffer, and the length of the buffer's
 *   contents.
 *
 * Returns:
 * - 1 of the function succeeded, and 0 if it did not.
 */
int qhi_process_hash(qhi *hash, void *context, qhi_int32t_write_buffer_apply_func int32t_apply_func, qhi_char_write_buffer_apply_func chars_apply_func)
{
	uint32_t    idx;
	uint32_t    elements_in_buffer = 0;
	int32_t     key_buffer[1024];

	// set signature
	key_buffer[0] = 0x4851 + (hash->value_type << 16) + (hash->key_type << 20);
	key_buffer[1] = hash->element_count;
	if (!int32t_apply_func(context, key_buffer, 2)) {
		return 0;
	}

	// add string keys
	if (hash->key_type == QHI_KEY_TYPE_STRING) {
		key_buffer[0] = hash->keys.count;
		key_buffer[1] = hash->bucket_count;
		if (!int32t_apply_func(context, key_buffer, 2)) {
			return 0;
		}
		if (!chars_apply_func(context, hash->keys.values, hash->keys.count)) {
			return 0;
		}
	}

	// add string values
	if (hash->value_type == QHI_VALUE_TYPE_STRING) {
		key_buffer[0] = hash->s.count;
		if (!int32t_apply_func(context, key_buffer, 1)) {
			return 0;
		}
		if (!chars_apply_func(context, hash->s.values, hash->s.count)) {
			return 0;
		}
	}

	// add data
	for (idx = 0; idx < hash->bucket_count; idx++)	{
		qhl *list = &(hash->bucket_list[idx]);
		qhb *p = list->head;
		qhb *n;

		if (p) {
			if (hash->key_type == QHI_KEY_TYPE_STRING) {
				key_buffer[elements_in_buffer] = idx;
				key_buffer[elements_in_buffer + 1] = list->size;

				if (!int32t_apply_func(context, key_buffer, 2)) {
					return 0;
				}
			}

			while(p) {
				n = p->next;

				add_to_buffer(hash, key_buffer, &elements_in_buffer, p);

				if (elements_in_buffer == 1024) {
					if (!int32t_apply_func(context, key_buffer, elements_in_buffer)) {
						return 0;
					}
					elements_in_buffer = 0;
				}

				p = n;
			}
		}

		if (elements_in_buffer > 0) {
			if (!int32t_apply_func(context, key_buffer, elements_in_buffer)) {
				return 0;
			}
			elements_in_buffer = 0;
		}
	}

	return 1;
}

/**
 * Saves a hash to a file pointed to by the file descriptor
 *
 * Parameters:
 * - hash: the set to write
 * - fd: a file descriptor that is suitable for reading to
 *
 * Returns:
 * - 1 on success, and 0 on failure
 */
int qhi_hash_save_to_file(int fd, qhi *hash)
{
	fd_apply_context ctxt;
	ctxt.fd = fd;
	return qhi_process_hash(hash, (void *) &ctxt, fd_write_int32t_apply_func, fd_write_chars_apply_func);
}
