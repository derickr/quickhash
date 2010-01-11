#include <stdint.h>

typedef uint32_t (*qha_t)(uint32_t key);

typedef struct _qhb {
	uint32_t     key;
	struct _qhb *next;
} qhb;

typedef struct _qhl {
	qhb      *head;
	qhb      *tail;
} qhl;

typedef struct _qhi {
	qha_t     hasher; // hash algorithm
	uint32_t  bucket_count;
	qhl      *bucket_list;
} qhi;

qhi *qhi_create(uint32_t size);
void qhi_free(qhi *hash);

void qhi_set_add(qhi *hash, uint32_t position, void *value);
int qhi_set_exists(qhi *hash, uint32_t position);
