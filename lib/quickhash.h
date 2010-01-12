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

	// for pre-allocating buckets
	int32_t   bucket_buffer_nr;
	uint32_t  bucket_buffer_pos;
	qhb     **bucket_buffer;
} qhi;


qhi *qhi_create(uint32_t size);
void qhi_free(qhi *hash);

int qhi_set_add(qhi *hash, int32_t position);
int qhi_set_exists(qhi *hash, int32_t position);

qhi *qhi_set_load_from_file(int fd);
int qhi_set_save_to_file(int fd, qhi *hash);
