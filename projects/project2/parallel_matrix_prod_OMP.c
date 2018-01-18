
# include <stdio.h>
# include <stdlib.h>
# include <omp.h>

/* Function that reads in matrix stored in binary file <filename>, then stores matrix
in <matrix>, number of rows in <NUM_ROWS> and number of columns in <NUM_COLS>. */
void read_matrix_bin(char* filename, double*** matrix, int* NUM_ROWS, int* NUM_COLS){
	FILE* file_ptr = fopen(filename,"rb");
	int i;

	if (!file_ptr){
		printf("Unable to open %s!\n", filename);
		return;
	}

	fread(NUM_ROWS, sizeof(int), 1, file_ptr);			// Read number of rows.
	fread(NUM_COLS, sizeof(int), 1, file_ptr);			// Read number of cols.

	*matrix = (double**)malloc((*NUM_ROWS) * sizeof(double*));						// Allocating for double pointer.
	(*matrix)[0] = (double*)malloc((*NUM_ROWS) * (*NUM_COLS) * sizeof(double));		// Allocating entire needed space.
	for (i = 1; i < (*NUM_ROWS); i++){
		(*matrix)[i] = (*matrix)[i - 1] + (*NUM_COLS);								// Ensures contiguous storage.
	}

	fread((*matrix)[0], sizeof(double), (*NUM_ROWS) * (*NUM_COLS), file_ptr);		// Read in the entire matrix.
}

/* Function that that takes matrix <matrix> and its dimensions and writes
the matrix's content out to a binary file <filename> in the same folder. */
void write_matrix_bin (char* filename, double** matrix, int NUM_ROWS, int NUM_COLS){
	FILE* file_ptr = fopen(filename, "wb");
	fwrite(&NUM_ROWS, sizeof(int), 1, file_ptr);
	fwrite(&NUM_COLS, sizeof(int), 1, file_ptr);
	fwrite(matrix[0], sizeof(double), NUM_ROWS * NUM_COLS, file_ptr);
	fclose(file_ptr);
}

/* Function that takes in a matrix and its dimensions and then prints the
content of the matrix in a 2D structure to the screen. Mostly used for testing. */
void display_matrix(double** matrix, int NUM_ROWS, int NUM_COLS){
	int i, j;

	for (i = 0; i < NUM_ROWS; i++){
		printf("\n");

		for (j = 0; j < NUM_COLS; j++){
			printf("%.2f\t", matrix[i][j]);
		}
	}

	printf("\n\n");
}

/* Function that allocates a contiguous 2D-matrix with dimensions <NUM_ROWS> and <NUM_COLS> */
void allocate_contiguous_matrix(double*** matrix, int NUM_ROWS, int NUM_COLS){
	int i;

	*matrix = (double**)malloc(NUM_ROWS * sizeof(double*));							// Allocating for double pointer.
	(*matrix)[0] = (double*)malloc(NUM_ROWS * NUM_COLS * sizeof(double));			// Allocating entire needed space.
	for (i = 1; i < NUM_ROWS; i++){
		(*matrix)[i] = (*matrix)[i - 1] + NUM_COLS;									// Ensures contiguous storage.
	}
}

/* Function that executes the local matrix product of <matrix_A> and <matrix_B> and stores result in local portion matrix_C. */
void matrix_multiplication(double** matrix_A, double** matrix_B, double*** matrix_C, int NUM_ROWS_A, int NUM_COLS_A, int NUM_COLS_B){
	int i, j, k;
	omp_set_dynamic(1);			// Allowing dynamic change of number of threads.
	omp_set_num_threads(4);		// Setting the number of threads (per procs).

	/* Initiating parallel region (with most variables shared) and spawning threads. */
	# pragma omp parallel default(shared) private(i, j, k)
	{
		/* Implementation of the formula given in the exercise.
		Sharing the workload between multiple threads. */
		# pragma omp for private(i, j, k)
		for (i = 0; i < NUM_ROWS_A; i++){
			for (j = 0; j < NUM_COLS_B; j++){
				(*matrix_C)[i][j] = 0;

				for (k = 0; k < NUM_COLS_A; k++){
					(*matrix_C)[i][j] += matrix_A[i][k] * matrix_B[k][j];
				}
			}
		}
	}
}

int main(int argc, char* argv[]){
	double **matrix_A, **matrix_B, **matrix_C;				// To store entire matrix A, B and C.
	int NUM_ROWS_A, NUM_COLS_A, NUM_ROWS_B, NUM_COLS_B;		// To store dimesnions of A and B.
	char* input_file_A = argv[1];							// File name for bin file contaning A.
	char* input_file_B = argv[2];							// File name for bin file contaning B.

	read_matrix_bin(input_file_A, &matrix_A, &NUM_ROWS_A, &NUM_COLS_A);		// Read in A and its dimesnions from binary file.
	read_matrix_bin(input_file_B, &matrix_B, &NUM_ROWS_B, &NUM_COLS_B);		// Read in B and its dimensions from binary file.

	allocate_contiguous_matrix(&matrix_C, NUM_ROWS_A, NUM_COLS_B);			// Allocate memory for matrix_C to store result.

	matrix_multiplication(matrix_A, matrix_B, &matrix_C, NUM_ROWS_A, NUM_COLS_A, NUM_COLS_B);	// Execute matrix product A * B = C.
	display_matrix(matrix_C, NUM_ROWS_A, NUM_COLS_B);											// Activate to verify result matrix C.
	write_matrix_bin("parallel_generated_c.bin", matrix_C, NUM_ROWS_A, NUM_COLS_B);				// Write result matrix C to binary file.

	return(0);
}



