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

/**
 * Hashes the key
 *
 * The algorithm is from: http://burtleburtle.net/bob/hash/integer.html
 *
 * Parameters:
 * - key, the key to be hashsed
 *
 * Returns:
 * - the hash key
 */
uint32_t qha_jenkins1(uint32_t key)
{
	key = (key ^ 61) ^ (key >> 16);
	key = key + (key << 3);
	key = key ^ (key >> 4);
	key = key * 0x27d4eb2d;
	key = key ^ (key >> 15);
	return key;
}

/**
 * Hashes the key
 *
 * The algorithm is from: http://www.concentric.net/~Ttwang/tech/inthash.htm
 *
 * Parameters:
 * - key, the key to be hashsed
 *
 * Returns:
 * - the hash key
 */
uint32_t qha_jenkins2(uint32_t key)
{
	key = (key+0x7ed55d16) + (key<<12);
	key = (key^0xc761c23c) ^ (key>>19);
	key = (key+0x165667b1) + (key<<5);
	key = (key+0xd3a2646c) ^ (key<<9);
	key = (key+0xfd7046c5) + (key<<3);
	key = (key^0xb55a4f09) ^ (key>>16);
	return key;
}

/**
 * 'Hashes' the key by passing it straight through
 *
 * Parameters:
 * - key, the key to be hashsed
 *
 * Returns:
 * - the hash key
 */
uint32_t qha_no_hash(uint32_t key)
{
	return key;
}

/**
 * Helper struct that contains the file descriptor for use with
 * qhi_process_set or qhi_process_hash.
 */
typedef struct _fd_write_apply_context {
	int fd;
} fd_write_apply_context;

/**
 * Function that is used by qhi_set_save_to_file as an apply function for
 * qhi_process_set or qhi_process_hash. It is called when the buffer is full,
 * and will write the buffer's contents to the filedescriptor that is
 * contained in the fd_write_apply_context struct.
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
	fd_write_apply_context *ctxt = (fd_write_apply_context*) context;

	if (write(ctxt->fd, buffer, elements * sizeof(int32_t)) != (elements * sizeof(int32_t))) {
		return 0;
	}
	return 1;
}

/**
 * Function that is used by qhi_set_save_to_file as an apply function for
 * qhi_process_set or qhi_process_hash. It is called when the buffer is full,
 * and will write the buffer's contents to the filedescriptor that is
 * contained in the fd_write_apply_context struct.
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
	fd_write_apply_context *ctxt = (fd_write_apply_context*) context;

	if (write(ctxt->fd, buffer, elements) != elements) {
		return 0;
	}
	return 1;
}


/**
 * Defines the number of buckets to pre-allocate
 */
#define QHB_BUFFER_PREALLOC_INC 1024

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

	tmp->value_type = QHI_VALUE_TYPE_INT;
	tmp->size = 1024;
	tmp->check_for_dupes = 0;
	tmp->memory.malloc = malloc;
	tmp->memory.calloc = calloc;
	tmp->memory.realloc = realloc;
	tmp->memory.free = free;
	tmp->hasher = qha_jenkins2;

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

	tmp->value_type = options->value_type;

	tmp->hasher = options->hasher;
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
inline uint32_t qhi_set_hash(qhi *hash, uint32_t key)
{
	uint32_t idx = hash->hasher(key);
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
static int find_bucket_from_list(qhl *list, int32_t key, qhb **bucket)
{
	if (!list->head) {
		// there is no bucket list for this hashed key
		return 0;
	} else {
		qhb *p = (qhb*) list->head;

		// loop over the elements in this bucket list to see if the key exists
		do {
			if (p->key == key) {
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
int qhi_set_add(qhi *hash, int32_t key)
{
	uint32_t idx;
	qhl     *list;
	qhb  *bucket;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	// check if we already have the key in the list if requested
	if (hash->options->check_for_dupes && find_bucket_from_list(list, key, NULL)) {
		return 0;
	}

	// create new bucket
	bucket = qhb_create(hash);
	if (!bucket) {
		return 0;
	}
	bucket->key = key;
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
static int delete_entry_from_list(qhl *list, int32_t key)
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
			if (current->key == key) {
				// if previous is not set, it's the first element in the list, so we just adjust head.
				if (!previous) {
					list->head = current->next;
				} else {
					previous->next = current->next;
				}
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
int qhi_set_delete(qhi *hash, int32_t key)
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

	if (!delete_entry_from_list(list, key)) {
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
int qhi_set_exists(qhi *hash, int32_t key)
{
	uint32_t idx;
	qhl     *list;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	return find_bucket_from_list(list, key, NULL);
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
		added += qhi_set_add(hash, buffer[i]);
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
qhi *qhi_set_load_from_file(int fd, qho *options)
{
	struct stat finfo;
	uint32_t    nr_of_elements, elements_read = 0;
	uint32_t    bytes_read;
	int32_t     key_buffer[1024];
	qhi        *tmp;

	if (fstat(fd, &finfo) != 0) {
		return NULL;
	}

	// if the filesize is not an increment of 4, abort
	if (finfo.st_size % 4 != 0) {
		return NULL;
	}
	nr_of_elements = finfo.st_size / 4;

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
		bytes_read = read(fd, &key_buffer, sizeof(key_buffer));
		qhi_set_add_elements_from_buffer(tmp, key_buffer, bytes_read / 4);
		elements_read += (bytes_read / 4);
	} while (elements_read < nr_of_elements);
	return tmp;
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
int qhi_process_set(qhi *hash, void *context, qhi_int32t_buffer_apply_func int32t_apply_func)
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
	fd_write_apply_context ctxt;
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
static void hash_add_value(qhi *hash, qhb *bucket, qhv value)
{
	switch (hash->value_type) {
		case QHI_VALUE_TYPE_INT:
			if (hash->i.count == hash->i.size) {
				hash->i.values = hash->options->memory.realloc(hash->i.values, (hash->i.size + QHB_BUFFER_PREALLOC_INC) * sizeof(int32_t));
				hash->i.size += QHB_BUFFER_PREALLOC_INC;
			}
			hash->i.values[hash->i.count] = value.i;
			bucket->value_idx = hash->i.count;
			hash->i.count++;
			break;

		case QHI_VALUE_TYPE_STRING: {
			size_t str_len = strlen(value.s);
			if (hash->s.count + str_len + 1 >= hash->s.size) {
				hash->s.values = hash->options->memory.realloc(hash->s.values, (hash->s.size + QHB_BUFFER_PREALLOC_INC));
				hash->s.size += QHB_BUFFER_PREALLOC_INC;
			}
			memcpy(hash->s.values + hash->s.count, value.s, str_len + 1);
			bucket->value_idx = hash->s.count;
			hash->s.count = hash->s.count + str_len + 1;
		} break;

		default:
			/* FAIL */
			break;
	}
}

/**
 * Adds a new bucket with key and value to the list
 *
 * Parameters:
 * - hash: A valid quickhash
 * - list: The list that corresponds to the hashed version of the key
 * - key: The entry's key
 * - value: The value belonging to the entry
 *
 * Returns:
 * - 1 if the element was added or 0 if the element couldn't be added
 */
static int qhi_add_value_to_list(qhi *hash, qhl *list, int32_t key, qhv value)
{
	qhb *bucket;

	// create new bucket
	bucket = qhb_create(hash);
	if (!bucket) {
		return 0;
	}
	bucket->key = key;
	bucket->next = NULL;
	hash_add_value(hash, bucket, value);

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
	return 1;
}

static int qhi_update_value_in_bucket(qhi *hash, qhb *bucket, qhv value)
{
	switch (hash->value_type) {
		case QHI_VALUE_TYPE_INT:
			hash->i.values[bucket->value_idx] = value.i;
			return 1;

		case QHI_VALUE_TYPE_STRING:
			hash_add_value(hash, bucket, value);
			break;

		default:
			/* FAIL */
			break;
	}
	return 0;
}

static int qhi_get_value_from_bucket(qhi *hash, qhb *bucket, qhv* value)
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
 * Adds a new element to the hash
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 * - value: The value
 *
 * Returns:
 * - 1 if the element was added or 0 if the element couldn't be added
 */
int qhi_hash_add(qhi *hash, int32_t key, qhv value)
{
	uint32_t idx;
	qhl     *list;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	// check if we already have the key in the list if requested
	if (hash->options->check_for_dupes && find_bucket_from_list(list, key, NULL)) {
		return 0;
	}

	return qhi_add_value_to_list(hash, list, key, value);
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
int qhi_hash_update(qhi *hash, int32_t key, qhv value)
{
	uint32_t idx;
	qhl     *list;
	qhb     *bucket;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	if (find_bucket_from_list(list, key, &bucket)) {
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
int qhi_hash_set(qhi *hash, int32_t key, qhv value)
{
	uint32_t idx;
	qhl     *list;
	qhb     *bucket;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	// check if we already have the key in the list if requested
	if (find_bucket_from_list(list, key, &bucket)) {
		// update
		return qhi_update_value_in_bucket(hash, bucket, value);
	} else {
		// add
		return qhi_add_value_to_list(hash, list, key, value) ? 2 : 0;
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
int qhi_hash_get(qhi *hash, int32_t key, qhv *value)
{
	uint32_t idx;
	qhl     *list;
	qhb     *bucket;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	if (find_bucket_from_list(list, key, &bucket)) {
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
		added += qhi_hash_add(hash, buffer[i], (qhv) buffer[i + 1]);
	}
	return added;
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
	struct stat finfo;
	uint32_t    nr_of_elements, elements_read = 0;
	uint32_t    bytes_read;
	int32_t     key_buffer[1024];
	qhi        *tmp;

	if (fstat(fd, &finfo) != 0) {
		return NULL;
	}

	// we read the first two int32s to see whether this is a special type of hash
	if (read(fd, &key_buffer, 2 * sizeof(int32_t)) != (2 * sizeof(int32_t))) {
		return NULL;
	}
	if (key_buffer[0] == -QHI_VALUE_TYPE_STRING) {
		// position 1 contains the string length now
		finfo.st_size = finfo.st_size - 2 - key_buffer[1];
		options->value_type = QHI_VALUE_TYPE_STRING;
	}

	// if the filesize is not an increment of 8 (4*key+value), abort
	if (finfo.st_size % 8 != 0) {
		return NULL;
	}
	nr_of_elements = finfo.st_size / 8;

	// override the nr of bucket lists if the size is still 0.
	options->size = qhi_normalize_size(options->size == 0 ? nr_of_elements : options->size);
#if DEBUG
	printf("Picking size: %u\n", options->size);
#endif

	// create the hash
	tmp = qhi_create(options);
	if (!tmp) {
		return NULL;
	}

	// read the strings if we have QHI_VALUE_TYPE_STRING
	if (tmp->value_type == QHI_VALUE_TYPE_STRING) {
		tmp->s.values = tmp->options->memory.malloc(key_buffer[1] + 1);
		read(fd, tmp->s.values, key_buffer[1]);
		tmp->s.values[key_buffer[1]] = '\0';
	}

	// read the elements (key and value idx) and add them to the hash
	do {
		bytes_read = read(fd, &key_buffer, sizeof(key_buffer));
		qhi_hash_add_elements_from_buffer(tmp, key_buffer, bytes_read / 4);
		elements_read += (bytes_read / 8);
	} while (elements_read < nr_of_elements);

	return tmp;
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
int qhi_process_hash(qhi *hash, void *context, qhi_int32t_buffer_apply_func int32t_apply_func, qhi_char_buffer_apply_func chars_apply_func)
{
	uint32_t    idx;
	uint32_t    elements_in_buffer = 0;
	int32_t     key_buffer[1024];

	if (hash->value_type == QHI_VALUE_TYPE_STRING) {
		key_buffer[0] = -QHI_VALUE_TYPE_STRING;
		key_buffer[1] = hash->s.count;
		if (!int32t_apply_func(context, key_buffer, 2)) {
			return 0;
		}
		if (!chars_apply_func(context, hash->s.values, hash->s.count)) {
			return 0;
		}
	}

	for (idx = 0; idx < hash->bucket_count; idx++)	{
		qhl    *list = &(hash->bucket_list[idx]);
		qhb *p = list->head;
		qhb *n;

		if (p) {
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
	}

	if (elements_in_buffer > 0) {
		if (!int32t_apply_func(context, key_buffer, elements_in_buffer)) {
			return 0;
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
	fd_write_apply_context ctxt;
	ctxt.fd = fd;
	return qhi_process_hash(hash, (void *) &ctxt, fd_write_int32t_apply_func, fd_write_chars_apply_func);
}
