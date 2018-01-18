# INF3380-Projects
Projects and exercises for the course "Parallell programming for scientific problems" (INF3380) at the University of Oslo. The repository contains both the two min projects as well as smaller exercises worked on throughout the semester.

## Example of results from project 1 (parallel image denoising)
<img src="https://github.com/jostbr/INF3380-Projects/blob/master/projects/project1/parallel/MonaLisa_NOISY.jpg" width="280"> <img src="https://github.com/jostbr/INF3380-Projects/blob/master/projects/project1/parallel/MonaLisa_DENOISED_PARALLEL.jpg" width="280"> <img src="https://github.com/jostbr/INF3380-Projects/blob/master/projects/project1/parallel/MonaLisa_PARTIONING_ILLUSTRATION.jpg" width="280">

The left image displays the original noisy Mona Lisa image. In the middle one can see the denoised version using a prallellized image denoising algorithm. The right image illustrates the partitioning of the image amongts parallell processes (in this case 8 processes). The filtering algorithm for denoising requires information from neighbouring pizels meaning care has to be taken along the boundaries where the image has been distributed among the processes. This means that the processes must send/receive information when computing the denoised image.
