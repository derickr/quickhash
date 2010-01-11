#include <stdint.h>

/**
 * Hasher algorithm
 */
typedef uint32_t (*qha_t)(uint32_t key);

/**
 * Hash buckets
 */
typedef struct _qhb {
	uint32_t     key;
	struct _qhb *next;
} qhb;

/**
 * List of hash buckets
 */
typedef struct _qhl {
	qhb      *head;
	qhb      *tail;
} qhl;

/**
 * Integer hash type
 */
typedef struct _qhi {
	qha_t     hasher; // hash algorithm
	uint32_t  bucket_count;
	qhl      *bucket_list;
} qhi;


qhi *qhi_create(uint32_t size);
void qhi_free(qhi *hash);

int qhi_set_add(qhi *hash, uint32_t position);
int qhi_set_exists(qhi *hash, uint32_t position);
