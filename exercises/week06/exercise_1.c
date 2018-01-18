# include <stdio.h>
# include <string.h>
# include <mpi.h>

int main(int argc, char* argv[]){
	int num_processes, my_rank;
	char my_string[80];
	int num_chars, i;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	sprintf(my_string, "String from process %d", my_rank);
	num_chars = strlen(my_string) + 1;

	char combined_string[num_processes * num_chars];	

	// send all strings to process 0.
	MPI_Gather(my_string, num_chars, MPI_CHAR, combined_string,
		num_chars, MPI_CHAR, 0, MPI_COMM_WORLD);

	// Print out strings from other processes.
	if (my_rank == 0){
		for (i = 0; i < num_processes; i++){
			printf("%.*s\n", num_chars, combined_string + num_chars * i);
		}
	}

	MPI_Finalize();

	return(0);
}
