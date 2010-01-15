#include <stdint.h>
#include <stdlib.h>

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
	uint32_t size;
	char     check_for_dupes;
	qha_t    hasher;
	qhm      memory;
} qho;

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

	qho      *options;
#if DEBUG
	uint32_t  collisions;
#endif
} qhi;

uint32_t qha_jenkins1(uint32_t key);
uint32_t qha_jenkins2(uint32_t key);
uint32_t qha_no_hash(uint32_t key);


qho *qho_create(void);
void qho_free(qho *options);

uint32_t qhi_normalize_size(uint32_t size);
qhi *qhi_create(qho *options);
void qhi_free(qhi *hash);

int qhi_set_add(qhi *hash, int32_t position);
int qhi_set_exists(qhi *hash, int32_t position);

uint32_t qhi_set_add_elements_from_buffer(qhi *hash, int32_t *buffer, uint32_t nr_of_elements);
qhi *qhi_set_load_from_file(int fd, qho *options);
int qhi_set_save_to_file(int fd, qhi *hash);
