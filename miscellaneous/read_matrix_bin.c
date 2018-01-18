
# include <stdio.h>
# include <stdlib.h>

/* Function that reads in matrix stored in binary file <filename>, then stores the
matrix in <matrix>, number of rows in <NUM_ROWS> and number of columns in <NUM_COLS>. */
void read_matrix_bin(char* filename, double*** matrix, int* NUM_ROWS, int* NUM_COLS){
	FILE* file_ptr = fopen(filename, "rb");				// Create file pointer and open file.
	int i;												// Counter variable for for-loop below.

	/* If file pointer is NULL-object then exit. */
	if (!file_ptr){
		printf("Unable to open %s!\n", filename);
		return;
	}

	fread(NUM_ROWS, sizeof(int), 1, file_ptr);			// Read number of rows in matrix.
	fread(NUM_COLS, sizeof(int), 1, file_ptr);			// Read number of cols in matrix.

	/* Memory allocation (2D contiguous array). */
	*matrix = (double**)malloc((*NUM_ROWS) * sizeof(double*));						// Allocating for double pointer.
	(*matrix)[0] = (double*)malloc((*NUM_ROWS) * (*NUM_COLS) * sizeof(double));		// Allocating entire needed space.
	for (i = 1; i < (*NUM_ROWS); i++){
		(*matrix)[i] = (*matrix)[i - 1] + (*NUM_COLS);								// Ensures contiguous storage.
	}

	fread((*matrix)[0], sizeof(double), (*NUM_ROWS) * (*NUM_COLS), file_ptr);		// Read in the entire matrix.
	fclose(file_ptr);																// Close file after reading.
}
