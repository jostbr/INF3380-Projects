
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <mpi.h>
# include </home/cyclone/INF3380/ME01/simple-jpeg/import_export_jpeg.h>

/* Structure for image information. Relative to the template, m
and n are substituted with image_height and image_width respectively.*/
typedef struct{
	float** image_data;
	int image_height;
	int image_width;
} image;

/* Defines height and width for u (of type image) as well as
Allocating 2D-array u.image_data for future storage of image.*/
void allocate_image(image* u, int m, int n){
	int i;
	u->image_height = m;
	u->image_width = n;
	u->image_data = (float**)malloc(m * sizeof(float*));

	for (i = 0; i < m; i++){
		u->image_data[i] = (float*)malloc(n * sizeof(float));
	}
}

/* Deallocates the 2D-array u.image_data and frees the memory.*/
void deallocate_image(image* u){
	int i, j;

	for (i = 0; i < u->image_height; i++){
		free(u->image_data[i]);
	}

	free(u->image_data);
}

/* Converts 1D-array image_chars (of type unsigned char) into
2D-array u.image_data (of type float) so computations can be done.*/
void convert_jpeg_to_image(const unsigned char* image_chars, image* u){
	int i, j;

	for (i = 0; i < u->image_height; i++){
		for (j = 0; j < u->image_width; j++){
			u->image_data[i][j] = (float)image_chars[i * u->image_width + j];
		}
	}
}


/* Division of image where the height is divided into num_procs parts
with different size depending on the remainder image_height % num_procs.*/
int get_my_height(int image_height, int num_procs, int my_rank){
	int even_dist_height = image_height / num_procs;
	int remainder_height = image_height % num_procs;
	int my_height;

	/* Distribute the remainder among the first
	(image_height % num_procs) processes.*/
	if (my_rank < remainder_height){
		my_height = even_dist_height + 1;
	}

	else {
		my_height = even_dist_height;
	}

	return(my_height);
}

/* Finds start index for process my_rank's portion of image_chars.*/
int get_my_start(int* sendcounts, int num_procs, int my_rank){
	int my_start = 0, i;

	/* my_start for process my_rank is the sum of elements of
	image_chars assigned to all processes with lower rank than my_rank.*/
	for (i = 0; i < num_procs; i++){
		if (i < my_rank){
			my_start += sendcounts[i];
		}
	}

	return(my_start);
}

/* Function that computes the average value of the pixels in the image. */
float get_pixel_sum(image* u, int image_height, int image_width){
	float sum_pixels = 0;
	int i, j;

	for (i = 0; i < image_height; i++){
		for (j = 0; j < image_width; j++){
			sum_pixels += (float)u->image_data[i][j];
		}
	}

	return(sum_pixels);
}

/* Function that computes the squared deviation from the mean of the pixels in the image. */
float get_dev_squared_sum(image* u, int image_height, int image_width, float avg){
	float dev_squared = 0;
	int i, j;

	for (i = 0; i < image_height; i++){
		for (j = 0; j < image_width; j++){
			dev_squared += ((float)u->image_data[i][j] - avg) * ((float)u->image_data[i][j] - avg);
		}
	}

	return(dev_squared);
}

/* Main function based on skeleton in template that takes command-line
arguments iters, kappa, input_jpeg_filename, output_jpeg_filename, and
calls the above defined functions to execute the image denoising process.*/
int main(int argc, char* argv[]){
	image u, u_bar;									// Type struct image.
	int image_height, image_width, num_comp;
	unsigned char* image_chars;
	unsigned char* my_image_chars;
	int iters = atoi(argv[3]);						// String to int.
	float kappa = atof(argv[4]);					// String to float.
	char* input_jpeg_filename = argv[5];
	char* output_jpeg_filename = argv[6];
	int num_procs, my_height, my_width, my_rank;	// "Parallel variables".

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Loads in JPEG to process 0 and stores it in image_chars. Also
	prints out information about the imported JPEG to the standard output.*/
	if (my_rank == 0){
		import_JPEG_file(input_jpeg_filename, &image_chars, &image_height, &image_width, &num_comp);
		printf("W: %d, H: %d, C: %d, Name: %s\n", image_width, image_height, num_comp, input_jpeg_filename);
	}

	MPI_Bcast(&image_height, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&image_width, 1, MPI_INT, 0, MPI_COMM_WORLD);

	my_height = get_my_height(image_height, num_procs, my_rank);
	my_width = image_width;							// Keeping the size in width.

	allocate_image(&u, my_height, my_width);		// Allocate memory.
	allocate_image(&u_bar, my_height, my_width);	// Allocate memory.

	/* Here declaring variables and subsequently doing communication between
	the processes; all prepearing for the MPI_Scatterv() command further down.*/
	int my_sendcount, my_start, i; // Num. of local chars and local start.
	int* sendcounts = (int*)malloc(num_procs * sizeof(int));
	int* start_indices = (int*)malloc(num_procs * sizeof(int));
	my_sendcount = my_height * my_width;
	my_start = 0;	// To get updated values in second for-loop downwards.

	/* Filling sendcounts and making sure each procs has a copy.*/
	for (i = 0; i < num_procs; i++){
		MPI_Gather(&my_sendcount, 1, MPI_INT, sendcounts, 1, MPI_INT, i, MPI_COMM_WORLD);
	}

	my_start = get_my_start(sendcounts, num_procs, my_rank);

	/* Filling start_indices and making sure each procs has a copy.*/
	for (i = 0; i < num_procs; i++){
		MPI_Gather(&my_start, 1, MPI_INT, start_indices, 1, MPI_INT, i, MPI_COMM_WORLD);
	}

	my_image_chars = (unsigned char*)malloc(my_sendcount * sizeof(unsigned char));

	/* Partitioning (with different size) image_chars between processes.*/
	MPI_Scatterv(image_chars, sendcounts, start_indices, MPI_UNSIGNED_CHAR, my_image_chars, my_sendcount, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	convert_jpeg_to_image(my_image_chars, &u);		// Creates image on each procs.

	/* ------------------------------------- Here starts code (extra) relevant for this exercise. -------------------------------------*/
	float average_pixel, total_pixel_sum, my_pixel_sum;										// Declaring some variables.
	float standard_deviation, total_dev_squared, my_dev_squared;												// Declaring some variables.
	my_pixel_sum = get_pixel_sum(&u, my_height, my_width);									// Find average pixel of local image.

	MPI_Reduce(&my_pixel_sum, &total_pixel_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);	// Reduce (sum) all local pixel sums and store at procs 0.

	if (my_rank == 0){
		average_pixel = total_pixel_sum / (image_height * image_width);		// Compute average pixel value.
		printf("average_pixel = %.2f\n", average_pixel);					// Print out value of average pixel.
	}

	MPI_Bcast(&average_pixel, 1, MPI_INT, 0, MPI_COMM_WORLD);

	my_dev_squared = get_dev_squared_sum(&u, my_height, my_width, average_pixel);
	MPI_Reduce(&my_dev_squared, &total_dev_squared, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);	// Reduce (sum) all local pixel sums and store at procs 0.
	MPI_Bcast(&total_dev_squared, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	printf("%.2f\n", total_dev_squared);

	if (my_rank == 0){
		standard_deviation = sqrt(total_dev_squared / (image_height * image_width));	// Computing the standard deviation on process 0.
		printf("standard_deviation = %.2f\n", standard_deviation);						// This would give approx 68.73 (seems too large).
	}
	/* ------------------------------------- Here ends code (extra) relevant for this exercise. -------------------------------------*/

	deallocate_image(&u);							// Freeing memory from image u.
	deallocate_image(&u_bar);						// Freeing memory from image u_bar.

	MPI_Finalize();

	return(0);
}
