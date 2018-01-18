
# include <stdio.h>
# include <stdlib.h>
# include <omp.h>
# define ARRAY_SIZE ((int) 4e7)

int main(int argc, char* argv[]){
	double* vector_a = (double*)malloc(ARRAY_SIZE * sizeof(double));
	double* vector_b = (double*)malloc(ARRAY_SIZE * sizeof(double));
	double dot_product = 0;
	int i;

	# pragma omp parallel default(shared) private(i)
	{
		# pragma omp for
		for (i = 0; i < ARRAY_SIZE; i++){
			vector_a[i] = 0.0000001 * i;
			vector_b[i] = vector_a[i] - 1;
		}

		# pragma omp for schedule(static) reduction(+:dot_product)
		for (i = 0; i < ARRAY_SIZE; i++){
			dot_product += vector_a[i] * vector_b[i];
		}
	}

	printf("%f\n", dot_product);

	free(vector_a);
	free(vector_b);

	return(0);
}

/* It seems both the default static and guided schedules performs equally good.
However the default dynamic is a lot slower; it uses over twice the time. But,
changing the chunk-size (with dynamic) it performs as well as the two others.
When that's said, the threee different schedules seems to perform about the
same for varying chunk-sizes, and within one schedule, the chunk-size doesn't
seem to make a noticable difference on the performance of the program. However
changing the chunk-size made a slight difference on the numerical value for the
scalar product. It seems that some round off calculations are sligtly different
depending on the chunk-sizes. */
