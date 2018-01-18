
------------------------------------------------------------------------------------------
File best viewed without word-wrap enabled due to manuel line changes.
------------------------------------------------------------------------------------------

Answer (all files) to assignment written by student: Jostein Brændshøi
------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------
* List of files in the folder and corresponding description:

	parallel_matrix_prod_MPI.c  --------  Parallel MPI (TASK 1, ALTERNATIVE 1).
	parallel_matrix_prod_OMP.c  --------  Parallel OMP (no task, but for comparison).
	parallel_matrix_prod_MPI_OMP.c  ----  Parallel MPI and OMP (TASK 2).
	serial_matrix_prod.c  --------------  All serial (no task, but for comparison).
	parallel_generated_c.bin  ----------  Current active name for parallel output binary.
	serial_generated_c.bin  ------------  Current active name for serial output binary.
	small_matrix_a.bin  ----------------  Given binary file contaning small A.
	small_matrix_b.bin  ----------------  Given binary file contaning small B.
	small_matrix_c.bin  ----------------  Given binary file contaning small C.
	large_matrix_a.bin  ----------------  Given binary file contaning large A.
	large_matrix_b.bin  ----------------  Given binary file contaning large B.
	large_matrix_c.bin  ----------------  Given binary file contaning large C.
  timing_results.txt  ----------------  Rough overiew of the performance of the programs.
  compile_run_commands.txt  ----------  List of compile/run cmds used for the programs.
	README.txt  ------------------------  This information file.

  All *.c-files listed above are executed within a virtual machine (OS: Ubuntu) running on
  4 processors. However each of the parallel programs can be run with any specified number
  of MPI-processes and/or threads.
------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------
* In task 1 I used the following commands to compile and run the program (similarly for
  large_matrix_a.bin etc.):

    ---$ mpicc parallel_matrix_prod_MPI.c -o parallel_matrix_prod_MPI

    ---$ mpirun -n <num_procs> ./parallel_matrix_prod_MPI -np <num_procs>
         small_matrix_a.bin small_matrix_b.bin

  When doing this we get the following relations between the command-line arguments
  and elements of the argv array:

    argv[0] = ./parallel_main           // Program name.
    argv[1] = -np                       // MPI argument.
    argv[2] = num_procs                 // MPI argument.
    argv[3] = small_matrix_a.bin        // 1st "actual" argument.
    argv[4] = small_matrix_b.bin        // 2nd "actual" argument.

  See delaration of input_file_A and input_file_B in parallel_matrix_prod_MPI.c for
  implementation of the above procedure for command line arguments.

  For the program parallel_matrix_prod_MPI_OMP.c in task 2, I used the following commands:

    ---$ mpicc -fopenmp parallel_matrix_prod_MPI_OMP.c -o parallel_matrix_prod_MPI_OMP

    ---$ mpirun -n <num_procs> ./parallel_matrix_prod_MPI_OMP -np <num_procs>
         small_matrix_a.bin small_matrix_b.bin

  See compile_run_commands.txt for all the commands listed for the respective programs.
------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------
* For task 1 I chose alternative 1 i.e. an initial rowwise distribution of the two input
  matrices A and B. Below is an outline/description of what the parallel program does
  (the description (and program itself) is for both small and large A, B):

  - First process 0 reads in the input matrices A and B from binary files (names specified
  	through command line) using the function given in the exercise (here it's called
  	read_matrix_bin()).
  -	Then the rows of A and B are divided as evenly as possible among the procs using
  	the function get_my_rows().
  - The send counts and start indices for both A and B (and C) are found for each process
  	and made available for all processes.
  - Based on the partioning found, A and B are then distributed rowwise as evenly as
  	possible to all the processes (using the MPI_Scatterv() function) yielding the initial
  	distribution described in alternative 1.
  - B is the made available to all processes (using MPI_Allgatherv()).
  - Then the the local matrix product between the local part of A and entire B is being
  	calculated, giving a local part of C at each process. This local part of C is rowwise,
  	meaning the amount of rows is the amount of rows in local part of A and the amount of
  	columns is the total amount of coloumns in B.
  - All part results of C (located on the different processes) is then being gathered
  	(using MPI_Gatherv()) on process 0.
  - Finally process 0 writes the result matrix C to a binary file.
------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------
* In task 2 OpenMP is used in the computation intensive parts, i.e., in the for-loop
  inside the matrix_muiltiplication() function. Since this function is called by every
  MPI-process, OpenMP is used on every MPI-process. Here the

    # pragma omp parallel default(shared) private(i, j, k)
    # pragma omp for private(i, j, k)

  directives are used to apply multithreading. The number of threads can be set inside the
  omp_set_thread_num() function.
------------------------------------------------------------------------------------------
