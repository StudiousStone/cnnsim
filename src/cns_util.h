#ifndef _CNS_UTIL_H_
#define _CNS_UTIL_H_

#include <stdlib.h>
#include <stdint.h>

typedef enum cns_bool_t {
	CNS_FLASE = 0,
	CNS_TRUE = 1
} cns_bool_t;

typedef enum cns_dtype {
	CNS_BOOL,
	CNS_FLOAT,
	CNS_INT32,
	CNS_INT16,
	CNS_INT8,
	CNS_UINT32,
	CNS_UINT16,
	CNS_UINT8
} cns_dtype;

#define cns_free free

void *cns_alloc(size_t size);
char *cns_path_alloc(size_t *sizep);
void *cns_clone(const void *src, size_t size);
void *cns_repeat(void *data, size_t size, int times);
int cns_compute_length(uint32_t ndim, uint32_t *dims);

#endif	/* _CNS_UTIL_H_ */