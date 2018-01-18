
# include <stdio.h>
# include <stdlib.h>
# include <mpi.h>

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

/* Function that divide the total number <NUM_ROWS> of rows in a matrix into
num_procs parts with different size depending on the remainder NUM_ROWS % num_procs. */
int get_my_rows(int NUM_ROWS, int num_procs, int my_rank){
	int even_dist_rows = NUM_ROWS / num_procs;		// Integer division to find the floor.
	int remainder_rows = NUM_ROWS % num_procs;		// Modulo operator to find the remainder.
	int my_rows;

	/* Distribute the remainder among the first (NUM_ROWS % num_procs) processes.*/
	if (my_rank < remainder_rows){
		my_rows = even_dist_rows + 1;
	}

	else {
		my_rows = even_dist_rows;
	}

	return(my_rows);
}

/* Function that executes the local matrix product of <my_matrix_A> and <matrix_B> and stores result in local portion my_matrix_C. */
void matrix_multiplication(double** my_matrix_A, double** matrix_B, double*** my_matrix_C, int my_rows_A, int NUM_COLS_A, int NUM_COLS_B){
	int i, j, k;

	/* Implementation of the formula given in the exercise. */
	for (i = 0; i < my_rows_A; i++){
		for (j = 0; j < NUM_COLS_B; j++){
			(*my_matrix_C)[i][j] = 0;

			for (k = 0; k < NUM_COLS_A; k++){
				(*my_matrix_C)[i][j] += my_matrix_A[i][k] * matrix_B[k][j];
			}
		}
	}
}

/* Main function reads in matrices A and B (on process 0) from the two binary files specified by argv[3] and argv[4]. Then distributes both A and B, as
evenly as possible using a 1D rowwise partioning, to the num_procs processes. Furthermore gathers all parts of B at all processes. Then computes the
local part of the result matrix C on each process. Finally gathers all part-results back to process 0 which then writes C to a binary file. */
int main(int argc, char* argv[]){
	int num_procs, my_rank;											// Number of procs and ID of each procs.

	MPI_Init(&argc, &argv);											// Initialize MPI session/section.
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);						// Get total number of processes.
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);						// Get the ID/rank of every process.

	double **matrix_A, **matrix_B, **matrix_C;						// To store entire A and C (for procs 0) and B (for all).
	double **my_matrix_A, **my_matrix_B, **my_matrix_C;				// To store local portion of matrix A, B and result C.
	int NUM_ROWS_A, NUM_COLS_A, NUM_ROWS_B, NUM_COLS_B, i, j;		// To store number of rows and columns in A and B
	int my_rows_A, my_rows_B, my_sendcount_A, my_sendcount_B;		// To store local num of rows/cols as well as send amount.
	char* input_file_A = argv[3];									// Input file name (from command line) that holds matrix A.
	char* input_file_B = argv[4];									// Input file name (from command line) that holds matrix B.

	int* sendcounts_A = (int*)malloc(num_procs * sizeof(int));		// Allocate memory for sendcounts array for matrix A.
	int* sendcounts_B = (int*)malloc(num_procs * sizeof(int));		// Allocate memory for sendcounts array for matrix B.
	int* sendcounts_C = (int*)malloc(num_procs * sizeof(int));		// Allocate memory for sendcounts array for matrix C.
	int* displs_A = (int*)malloc(num_procs * sizeof(int));			// Allocate memory for displacement array for matrix A.
	int* displs_B = (int*)malloc(num_procs * sizeof(int));			// Allocate memory for displacement array for matrix B.
	int* displs_C = (int*)malloc(num_procs * sizeof(int));			// Allocate memory for displacement array for matrix C.

	/* Read in matrix A and B on process 0. */
	if (my_rank == 0){
		read_matrix_bin(input_file_A, &matrix_A, &NUM_ROWS_A, &NUM_COLS_A);				// Store A in matrix_A and dims. in NUM_ROWS_A, NUM_COLS_A.
		read_matrix_bin(input_file_B, &matrix_B, &NUM_ROWS_B, &NUM_COLS_B);				// Store A in matrix_B and dims. in NUM_ROWS_B, NUM_COLS_B.
	}

	MPI_Bcast(&NUM_ROWS_A, 1, MPI_INT, 0, MPI_COMM_WORLD);		// Broadcast (to all, from process 0) the number of rows in A.
	MPI_Bcast(&NUM_COLS_A, 1, MPI_INT, 0, MPI_COMM_WORLD);		// Broadcast (to all, from process 0) the number of cols in A.
	MPI_Bcast(&NUM_ROWS_B, 1, MPI_INT, 0, MPI_COMM_WORLD);		// Broadcast (to all, from process 0) the number of rows in B.
	MPI_Bcast(&NUM_COLS_B, 1, MPI_INT, 0, MPI_COMM_WORLD);		// Broadcast (to all, from process 0) the number of cols in B.

	my_rows_A = get_my_rows(NUM_ROWS_A, num_procs, my_rank);	// Find my_rank's amount of rows (based on as even dist. as possible) in A.
	my_rows_B = get_my_rows(NUM_ROWS_B, num_procs, my_rank);	// Find my_rank's amount of rows (based on as even dist. as possible) in B.
	my_sendcount_A = my_rows_A * NUM_COLS_A;					// The total number of elements (of A) to be sent from procs 0 to my_rank.
	my_sendcount_B = my_rows_B * NUM_COLS_B;					// The total number of elements (of B) to be sent from procs 0 to my_rank.

	MPI_Allgather(&my_sendcount_A, 1, MPI_INT, sendcounts_A, 1, MPI_INT, MPI_COMM_WORLD);	// Fill sendcounts (for A) and make available to every procs.
	MPI_Allgather(&my_sendcount_B, 1, MPI_INT, sendcounts_B, 1, MPI_INT, MPI_COMM_WORLD);	// Fill sendcounts (for B) and make available to every procs.

	/* Fill sendcounts (for C) and fill displs (for A, B and C). */
	for (i = 0; i < num_procs; i++){
		sendcounts_C[i] = (sendcounts_A[i] / NUM_COLS_A) * NUM_COLS_B;	// Num of elements for procs i to be sent back to procs 0 later.
		displs_A[i] = 0;												// Initialize displs_A[i] to be zero (i = 0 -> displs_A[0] = 0 (all j)).
		displs_B[i] = 0;												// Initialize displs_B[i] to be zero (i = 0 -> displs_B[0] = 0 (all j)).
		displs_C[i] = 0;												// Initialize displs_A[i] to be zero (i = 0 -> displs_C[0] = 0 (all j)).

		/* Start index for procs i is the sum of sendcounts up to i - 1. */
		for (j = 1; j < num_procs; j++){
			if (j <= i){
				displs_A[i] += sendcounts_A[j - 1];						// Computing start indices (in A) cumulatively for process number i.
				displs_B[i] += sendcounts_B[j - 1];						// Computing start indices (in B) cumulatively for process number i.
				displs_C[i] += sendcounts_C[j - 1];						// Computing start indices (in C). This is where process i stores its part of C.
			}
		}
	}

	/* Procs 0 has already allocated and stored A and B. */
	if (my_rank != 0){
		allocate_contiguous_matrix(&matrix_A, NUM_ROWS_A, NUM_COLS_A);	// Need to allocate some mem (for A) for all other procs to use Scatterv properly.
		allocate_contiguous_matrix(&matrix_B, NUM_ROWS_B, NUM_COLS_B);	// Need to allocate some mem (for A) for all other procs to use Scatterv properly.
	}

	allocate_contiguous_matrix(&my_matrix_A, my_rows_A, NUM_COLS_A);	// Allocate memory to hold local portion of A (based on my_rows_A).
	allocate_contiguous_matrix(&my_matrix_B, my_rows_B, NUM_COLS_B);	// Allocate memory to hold local portion of B (based on my_rows_B).

	/* Distribute rowwise A and B (from procs 0) to all procs and store my_rank's portion of A and B in my_matrix_A and my_matrix_B. */
	MPI_Scatterv(&matrix_A[0][0], sendcounts_A, displs_A, MPI_DOUBLE, &my_matrix_A[0][0], my_sendcount_A, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatterv(&matrix_B[0][0], sendcounts_B, displs_B, MPI_DOUBLE, &my_matrix_B[0][0], my_sendcount_B, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	/* Gather all parts of B (located on the different procs) to all procs such that every procs has access to all of B. Store in matrix_B. */
	MPI_Allgatherv(&my_matrix_B[0][0], my_sendcount_B, MPI_DOUBLE, &matrix_B[0][0], sendcounts_B, displs_B, MPI_DOUBLE, MPI_COMM_WORLD);

	/* Allocate memory for local portion of result my_matrix_C. Then execute local matrix product and store result here. */
	allocate_contiguous_matrix(&my_matrix_C, my_rows_A, NUM_COLS_B);								// Dims of local C is local rows in A times cols in B.
	matrix_multiplication(my_matrix_A, matrix_B, &my_matrix_C, my_rows_A, NUM_COLS_A, NUM_COLS_B);	// Every procs ends up with a rowwise part of matrix_C.

	/* Allocate memory for entire result matrix C. Then gather all local parts of C into matrix_C stored on procs 0. */
	allocate_contiguous_matrix(&matrix_C, NUM_ROWS_A, NUM_COLS_B);
	MPI_Gatherv(&my_matrix_C[0][0], sendcounts_C[my_rank], MPI_DOUBLE, &matrix_C[0][0], sendcounts_C, displs_C, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	/* Procs 0 writes entire result of matrix product A * B = C to binary file. */
	if (my_rank == 0){
		//display_matrix(matrix_C, NUM_ROWS_A, NUM_COLS_B);									// Display matrix_C (on process 0) for verification.
		write_matrix_bin("parallel_generated_c.bin", matrix_C, NUM_ROWS_A, NUM_COLS_B);	// Write matrix_C to binary file with name of choice.
	}

	free(sendcounts_A); free(sendcounts_B); free(sendcounts_C);		// Deallocate memory for sendcounts arrays for all matrices.
	free(displs_A); free(displs_B); free(displs_C);					// Deallocate memory for displs arrays for all matrices.
	free(matrix_A[0]); free(matrix_A);								// Deallocate mem first for elements, then for double pointer.
	free(matrix_B[0]); free(matrix_B);								// Deallocate mem first for elements, then for double pointer.
	free(matrix_C[0]); free(matrix_C);								// Deallocate mem first for elements, then for double pointer.
	free(my_matrix_A[0]); free(my_matrix_A);						// Deallocate mem first for elements, then for double pointer.
	free(my_matrix_B[0]); free(my_matrix_B);						// Deallocate mem first for elements, then for double pointer.
	free(my_matrix_C[0]); free(my_matrix_C);						// Deallocate mem first for elements, then for double pointer.

	MPI_Finalize();
	return(0);
}
