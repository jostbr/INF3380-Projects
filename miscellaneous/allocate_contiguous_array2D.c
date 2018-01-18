
# include <stdio.h>
# include <stdlib.h>

/* Function that allocates a contiguous 2D-array with dimensions <NUM_ROWS> and <NUM_COLS>. */
void allocate_contiguous_array2D(double*** matrix, int NUM_ROWS, int NUM_COLS){
	int i;

	*matrix = (double**)malloc(NUM_ROWS * sizeof(double*));							// Allocating for double pointer.
	(*matrix)[0] = (double*)malloc(NUM_ROWS * NUM_COLS * sizeof(double));			// Allocating entire needed space.

	for (i = 1; i < NUM_ROWS; i++){
		(*matrix)[i] = (*matrix)[i - 1] + NUM_COLS;									// Ensures contiguous storage.
	}
}
