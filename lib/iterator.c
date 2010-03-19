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

#include "quickhash.h"

/**
 * Initializes an iterator object
 *
 * This also protects deletings from the hash. qhi_iterator_deinit should be
 * called when you're done iterating so that you can again delete elements from
 * the hash.
 *
 * Parameters:
 * - iter: a pointer to an existing iterator object (usually fed in with &iter).
 * - hash: a valid quickhash
 */
void qhi_iterator_init(qhit *iter, qhi *hash)
{
	iter->hash = hash;
	iter->key = 0;
	iter->value = 0;
	iter->bucket_list_idx = 0;
	iter->current_bucket = NULL;

	iter->hash->iterator_count++;
}

/**
 * Iterates of the bucketlists until it finds a non-empty one.
 *
 * Parameters:
 * - iter: a pointer to an initialized iterator object
 *
 * Returns:
 * - 1 when a new bucket was found, or 0 when the end of the lists was reached.
 */
static int forward_to_bucket_list(qhit *iter)
{
	/* Loop til we either reached the end of the lists, or when we find a non-empty list */
	while (iter->bucket_list_idx < iter->hash->bucket_count && !iter->hash->bucket_list[iter->bucket_list_idx].head) {
		iter->bucket_list_idx++;
	}
	/* If we're out of lists, signal that there are no more lists */
	if (iter->bucket_list_idx == iter->hash->bucket_count) {
		return 0;
	}
	return 1;
}

static void read_values(qhit **iter)
{
	switch ((*iter)->hash->value_type) {
		case QHI_VALUE_TYPE_INT:
			if ((*iter)->hash->i.values) {
				(*iter)->value = (*iter)->hash->i.values[((qhb*) (*iter)->current_bucket)->value_idx];
			}
			break;

		case QHI_VALUE_TYPE_STRING:
			break;

		default:
			break;
	}
}

/**
 * Iterates to the next bucket and sets the value and key elements of the
 * iterator to ones belonging to that bucket.
 *
 * Parameters:
 * - iter: a pointer to an initialized iterator object
 *
 * Returns:
 * - 1 when a new bucket was found, or 0 when there were no more buckets
 */
int qhi_iterator_forward(qhit *iter)
{
	if (iter->current_bucket == NULL) {
		/* First entry, so we search for the first key */
		if (forward_to_bucket_list(iter)) {
			iter->current_bucket = iter->hash->bucket_list[iter->bucket_list_idx].head;
			iter->key = iter->current_bucket->key;
			read_values((qhit**) &iter);
			return 1;
		}
	} else {
		/* Check if there are more buckets in this list */
		if (iter->current_bucket->next) {
			iter->current_bucket = iter->current_bucket->next;
			iter->key = iter->current_bucket->key;
			read_values((qhit**) &iter);
			return 1;
		}

		/* If not, go to the next bucket if it exists */
		iter->bucket_list_idx++;
		if (forward_to_bucket_list(iter)) {
			iter->current_bucket = iter->hash->bucket_list[iter->bucket_list_idx].head;
			iter->key = iter->hash->bucket_list[iter->bucket_list_idx].head->key;
			read_values((qhit**) &iter);
			return 1;
		}
	}
	return 0;
}

/**
 * Deinitializes an iterator.
 *
 * This should be called when you're done iterating so that you can again
 * delete elements from the hash.
 *
 * Parameters:
 * - iter: a pointer to an initialized iterator object
 *
 */
void qhi_iterator_deinit(qhit *iter)
{
	iter->hash->iterator_count--;
}
