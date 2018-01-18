# Projects in INF3380
Projects and exercises for the course "Parallell programming for scientific problems" (INF3380) at the University of Oslo. The repository contains both the two min projects as well as smaller exercises worked on throughout the semester.

## Example of results from project 1 (parallel image denoising)
<img src="https://github.com/jostbr/INF3380-Projects/blob/master/projects/project1/parallel/MonaLisa_NOISY.jpg" width="280"> <img src="https://github.com/jostbr/INF3380-Projects/blob/master/projects/project1/parallel/MonaLisa_DENOISED_PARALLEL.jpg" width="280"> <img src="https://github.com/jostbr/INF3380-Projects/blob/master/projects/project1/parallel/MonaLisa_PARTIONING_ILLUSTRATION.jpg" width="280">

The left image displays the original noisy Mona Lisa image. In the middle one can see the denoised version using a prallelized image denoising algorithm using both MPI and OpenMP. The right image illustrates the partitioning of the image amongt parallel MPI processes (in this case 8 processes). The filtering algorithm for denoising requires information from neighbouring pizels meaning care has to be taken along the boundaries where the image has been distributed among the processes. This means that the processes must send/receive information when computing the denoised image.

## Project 2
The second project involved writing a parallel algorithm for the matrix product of two matrices given in binary files. This was accomplished by row-wise distribution of the two matrices among MPI processes. Each process then computes a local part of the result matrix using OpenMP shared memory parallelization. Finally, all local parts of the result matrix is gathered at the master process and stistched together before written to binary file. The parallel algorithm showed, as with project 1, significant speedupå over its serialized counterpart.
