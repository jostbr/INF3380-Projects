
# include <stdio.h>
# include <stdlib.h>
# include <mpi.h>
# define ARRAY_SIZE 64

/* Function that allocates a 1D-array of length SIZE. */
void allocate_vector(double** vector_x, int SIZE){
	*vector_x = (double*)malloc(SIZE * sizeof(double));
}

/* Function that allocates a contiguous 2D-matrix with dimensions <NUM_ROWS> and <NUM_COLS> */
void allocate_contiguous_matrix(double*** matrix, int NUM_ROWS, int NUM_COLS){
	int i;

	*matrix = (double**)malloc(NUM_ROWS * sizeof(double*));
	(*matrix)[0] = (double*)malloc(NUM_ROWS * NUM_COLS * sizeof(double));
	for (i = 1; i < NUM_ROWS; i++){
		(*matrix)[i] = (*matrix)[i - 1] + NUM_COLS;
	}
}

/* Function that executes the local matrix product of <matrix_A> and <matrix_B> and stores result in local portion matrix_C. */
void matrix_multiplication(double** matrix_A, double** matrix_B, double*** matrix_C, int NUM_ROWS_A, int NUM_COLS_A, int NUM_COLS_B){
	int i, j, k;

	for (i = 0; i < NUM_ROWS_A; i++){
		for (j = 0; j < NUM_COLS_B; j++){
			(*matrix_C)[i][j] = 0;
			
			for (k = 0; k < NUM_COLS_A; k++){
				(*matrix_C)[i][j] += matrix_A[i][k] * matrix_B[k][j];
			}
		}
	}
}

int main(int argc, char* argv[]){
	int num_procs, my_rank, i, j;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	double **matrix_A, *vector_x;

	if (my_rank == 0){
		allocate_contiguous_matrix(&matrix_A, ARRAY_SIZE, ARRAY_SIZE);
		allocate_vector(&vector_x, ARRAY_SIZE);

		/* Initialize matrix and vector. */
		for (i = 0; i < ARRAY_SIZE; i++){
			vector_x[i] = i;

			for (j = 0; j < ARRAY_SIZE; j++){
				matrix_A[i][j] = i + j;
			}
		}
	}

	/* Aquire wanted starting point for data. */
	




	MPI_Finalize();

	return(0);
}
