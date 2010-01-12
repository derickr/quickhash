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
static uint32_t jenkins(uint32_t key)
{
    key = (key ^ 61) ^ (key >> 16);
    key = key + (key << 3);
    key = key ^ (key >> 4);
    key = key * 0x27d4eb2d;
    key = key ^ (key >> 15);
    return key;
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
	qhb *tmp = NULL;

	if (hash->bucket_buffer_pos % QHB_BUFFER_PREALLOC_INC == 0) {
		hash->bucket_buffer_nr++;
		hash->bucket_buffer = realloc(hash->bucket_buffer, sizeof(qhb*) * (hash->bucket_buffer_nr + 1));
		if (!hash->bucket_buffer) {
			return NULL;
		}
		hash->bucket_buffer[hash->bucket_buffer_nr] = malloc(sizeof(qhb) * QHB_BUFFER_PREALLOC_INC);
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
 * Creates a new integer quick hash
 *
 * Parameters:
 * - options: the options to create the hash with. This structure contains at
 *   least the nr of hash buckets, and whether set additions should be checked
 *   for duplicates. See the description of qho for a full list of options.
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

	if (options->size < 4) {
		return NULL;
	}
	if (options->size < 1048576) {
		size = options->size;
	} else {
		size = 1048576;
	}

	tmp = malloc(sizeof(qhi));
	if (!tmp) {
		return NULL;
	}

	tmp->hasher = jenkins;
	tmp->bucket_count = size;
	tmp->bucket_list = calloc(sizeof(qhl) * size, 1);

	tmp->bucket_buffer_nr   = -1;
	tmp->bucket_buffer_pos  = 0;
	tmp->bucket_buffer      = NULL;

	if (!tmp->bucket_list) {
		free(tmp);
		return NULL;
	}

	tmp->options = options;

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
	uint32_t idx;

	for (idx = 0; idx <= hash->bucket_buffer_nr; idx++) {
		free(hash->bucket_buffer[idx]);
	}

	free(hash->bucket_buffer);
	free(hash->bucket_list);
	free(hash);
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
	return idx % hash->bucket_count;
}

/**
 * Internal: checks whether a key exists in a specific list
 *
 * Parameters:
 * - list: the list belonging to the index of the hashed key
 * - key: the element's key
 *
 * Returns:
 * - 1 if the key exists in the list, 0 if not
 */
static int find_entry_in_list(qhl *list, int32_t key)
{
	if (!list->head) {
		// there is no bucket list for this hashed key
		return 0;
	} else {
		qhb *p = list->head;

		// loop over the elements in this bucket list to see if the key exists
		do {
			if (p->key == key) {
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
	qhb     *bucket;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	// check if we already have the key in the list if requested
	if (hash->options->check_for_dupes && find_entry_in_list(list, key)) {
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
	}
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

	return find_entry_in_list(list, key);
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
	uint32_t    i, bytes_read;
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

	// override the nr of bucket lists as we know better
	options->size = nr_of_elements;

	// create the set
	tmp = qhi_create(options);
	if (!tmp) {
		return NULL;
	}

	// read the elements and add them to the set
	do {
		bytes_read = read(fd, &key_buffer, sizeof(key_buffer));
		for (i = 0; i < bytes_read / 4; i++) {
			qhi_set_add(tmp, key_buffer[i]);
		}
		elements_read += (bytes_read / 4);
	} while (elements_read < nr_of_elements);
	return tmp;
}

/**
 * Saves a set to a file point to by the file descriptor
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
	uint32_t    idx;
	uint32_t    elements_in_buffer = 0;
	int32_t     key_buffer[1024];

	for (idx = 0; idx < hash->bucket_count; idx++)	{
		qhl *list = &(hash->bucket_list[idx]);
		qhb *p = list->head;
		qhb *n;

		if (p) {
			while(p) {
				n = p->next;

				key_buffer[elements_in_buffer] = p->key;
				elements_in_buffer++;

				if (elements_in_buffer == 1024) {
					if (write(fd, key_buffer, elements_in_buffer * 4) != (elements_in_buffer * 4)) {
						return 0;
					}
					elements_in_buffer = 0;
				}

				p = n;
			}
		}
	}

	if (elements_in_buffer > 0) {
		if (write(fd, key_buffer, elements_in_buffer * 4) != (elements_in_buffer * 4)) {
			return 0;
		}
	}
	return 1;
}
