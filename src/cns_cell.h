#ifndef _CNS_CELL_H_
#define _CNS_CELL_H_

#include <stdint.h>
#include <stdio.h>
#include <limits.h>

#define CNS_MAX_CELLS LONG_MAX;

typedef struct cns_cell_data {
	uint8_t        input_width;
	uint8_t        output_width;
	uint8_t        weight_width;

	int            input_dtype;
	int            output_dtype;
	int            weight_dtype;

	void           *input;
	void           *output;
	void           *weight;
} cns_cell_data;

typedef void (* cns_cell_op) (cns_cell_data *data);

typedef struct cns_cell {
	cns_cell_data  data;
	cns_cell_op    op;
	size_t         index;	/* index in a cell array */

	/* Indexes of cells in the same cell array on which this cell depends.
	   It starts running after those cells all stop running.
	   -1 is a special index. Every cell running in the first place should
	   depend on and only on -1.
	   NOTICE: Avoid dead locks */
	ssize_t        *dep;
} cns_cell;

typedef struct cns_cell_array {
	size_t         size;
	cns_cell       *cells;
} cns_cell_array;

void cns_cell_run(cns_cell *cell);
void cns_cell_set_cell_data(cns_cell *cell, cns_cell_data *cell_data);
void cns_cell_set_data(cns_cell *cell, void *input, void *output, void *weight);
void cns_cell_set_width(cns_cell *cell, uint8_t input_width,
			uint8_t output_width, uint8_t weight_width);
void cns_cell_set_dtype(cns_cell *cell, int input_dtype,
			int output_dtype, int weight_dtype);
void cns_cell_set_op(cns_cell *cell, cns_cell_op op);
void cns_cell_set_dep(cns_cell *cell, ssize_t *dep);
void cns_cell_fprint_data(FILE *fp, cns_cell *cell);
void cns_cell_print_data(cns_cell *cell);

cns_cell_array *cns_cell_array_create(size_t size);
void cns_cell_array_free(cns_cell_array *cell_array);
void cns_cell_array_run(cns_cell_array *cell_array);

#endif	/* _CNS_CELL_H_ */