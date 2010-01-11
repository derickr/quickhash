#include <malloc.h>
#include "quickhash.h"

/**
 * Generic integer set implementation
 *
 * mem use:
 * 64bit machine: 16*size + 24*item + 24
 * 32bit machine:  8*size + 12*item + 12
 *
 * allocations:
 * 2 + item
 */


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
 * Allocates a hash bucket.
 *
 * It is separated into its own function so that we potentionally can do
 * something clever here and pre-allocate a whole lot of stuff.
 *
 * Returns:
 * - A newly allocated hash bucket or NULL upon allocation failure
 */
inline qhb *qhb_create(void)
{
	qhb *tmp = calloc(sizeof(qhb), 1);
	return tmp;
}

/**
 * Creates a new integer quick hash
 *
 * Parameters:
 * - size: the number of buckets to use. A typical value to pick here is to
 *         pick the expected amount of set elements.
 *
 * Returns:
 * - a pointer to the hash, or NULL if:
 *   - upon memory allocation failures.
 *   - size < 4
 */
qhi *qhi_create(uint32_t size)
{
	qhi *tmp;

	if (size < 4) {
		return NULL;
	}

	tmp = malloc(sizeof(qhi));
	if (!tmp) {
		return NULL;
	}

	tmp->hasher = jenkins;
	tmp->bucket_count = size;
	tmp->bucket_list = calloc(sizeof(qhl) * size, 1);

	if (!tmp->bucket_list) {
		free(tmp);
		return NULL;
	}

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

	for (idx = 0; idx < hash->bucket_count; idx++)	{
		qhl *list = &(hash->bucket_list[idx]);
		qhb *p = list->head;
		qhb *n;

		if (p) {
			while(p) {
				n = p->next;
				free(p);
				p = n;
			}
		}
	}
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
 * Adds a new element to the hash
 *
 * Parameters:
 * - hash: A valid quickhash
 * - key: The key
 *
 * Returns:
 * - 1 if the element was added or 0 if the element couldn't be added
 */
int qhi_set_add(qhi *hash, uint32_t key)
{
	uint32_t idx;
	qhl     *list;
	qhb     *bucket;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

	// create new bucket
	bucket = qhb_create();
	if (!bucket) {
		return 0;
	}
	bucket->key = key;

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
int qhi_set_exists(qhi *hash, uint32_t key)
{
	uint32_t idx;
	qhl     *list;

	// obtain the hashed key, and the bucket list for the hashed key
	idx = qhi_set_hash(hash, key);
	list = &(hash->bucket_list[idx]);

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
