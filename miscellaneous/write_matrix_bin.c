
# include <stdio.h>
# include <stdlib.h>

/* Function that takes matrix <matrix> together with its number of
rows and cols and stores the data in binary file specified by <filename>. */
void write_matrix_bin (char* filename, double** matrix, int NUM_ROWS, int NUM_COLS){
	FILE* file_ptr = fopen(filename, "wb");								// Create file pointer and open file.
	fwrite(&NUM_ROWS, sizeof(int), 1, file_ptr);						// Write number of rows in matrix.
	fwrite(&NUM_COLS, sizeof(int), 1, file_ptr);						// Write number of cols in matrix.
	fwrite(matrix[0], sizeof(double), NUM_ROWS * NUM_COLS, file_ptr);	// Write matrix elements to file.
	fclose(file_ptr);													// Close file after writing.
}
