#include <malloc.h>
#include "quickhash.h"


uint32_t jenkins(uint32_t a)
{
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

// separated into its own function so that we potentionally can do something
// clever here and pre-allocate a whole lot of stuff.
inline qhb *qhb_create(void)
{
	qhb *tmp = calloc(sizeof(qhb), 1);
	return tmp;
}

// mem use:
// 64bit machine: 16*size + 24*item + 24
// 32bit machine:  8*size + 12*item + 12

// allocations:
// 2 + item
qhi *qhi_create(uint32_t size)
{
	qhi *tmp = malloc(sizeof(qhi));
	tmp->hasher = jenkins;
	tmp->bucket_count = size;
	tmp->bucket_list = calloc(sizeof(qhl) * size, 1);
	return tmp;
}

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

inline uint32_t qhi_set_hash(qhi *hash, uint32_t position)
{
	uint32_t idx = hash->hasher(position);
	return idx % hash->bucket_count;
}

void qhi_set_add(qhi *hash, uint32_t position, void *value)
{
	uint32_t  idx = qhi_set_hash(hash, position);
	qhl *list = &(hash->bucket_list[idx]);

	// create new bucket
	qhb *bucket = qhb_create();
	bucket->key   = position;

	// add bucket to list
	if (list->head == NULL) {
		printf("New list for %d\n", idx);
		// first bucket in list
		list->head = bucket;
		list->tail = bucket;
	} else {
		printf("Adding to list for %d\n", idx);
		list->tail->next = bucket;
		list->tail = bucket;
	}
}

int qhi_set_exists(qhi *hash, uint32_t position)
{
	uint32_t  idx = qhi_set_hash(hash, position);
	qhl *list = &(hash->bucket_list[idx]);

	if (!list->head) {
		return 0;
	} else {
		qhb *p = list->head;

		do {
			if (p->key == position) {
				return 1;
			}
			p = p->next;
		} while(p);
	}
	return 0;
}
