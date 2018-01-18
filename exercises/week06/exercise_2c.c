# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <mpi.h>

int main(int argc, char* argv[]){
	int num_processes, my_rank, i;
	int num_random = atoi(argv[3]);
	int rand_lim = atoi(argv[4]);
	int local_sum = 0;
	int total_sum;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	srand(time(NULL) * (my_rank + 1));	// Unique random for every process

	for (i = 0; i < num_random; i++){
		local_sum += rand() % rand_lim;
	}

	printf("Local sum from rank %d is %d\n", my_rank, local_sum);

	MPI_Reduce(&local_sum, &total_sum, 1,
		MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (my_rank == 0){
		printf("Total sum on process 0 is: %d\n", total_sum);
	}

	MPI_Finalize();

	return(0);
}