# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <mpi.h>

int main(int argc, char* argv[]){
	int num_processes, my_rank, i;
	int num_random = atoi(argv[3]);
	int rand_lim = atoi(argv[4]);
	int local_sum = 0;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	srand(time(NULL) * (my_rank + 1));	// Unique random for every process

	for (i = 0; i < num_random; i++){
		local_sum += rand() % rand_lim;
	}

	printf("Local sum from rank %d is %d\n", my_rank, local_sum);

	if (my_rank != 0){
		MPI_Send(&local_sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}

	if (my_rank == 0){
		int current_local_sum;
		int total_sum = local_sum;
		MPI_Status status;

		for (i = 1; i < num_processes; i++){
			MPI_Recv(&current_local_sum, 1, MPI_INT,
				MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			total_sum += current_local_sum;
		}

		printf("Total sum computed on process 0 is: %d\n", total_sum);
	}

	MPI_Finalize();

	return 0;
}
