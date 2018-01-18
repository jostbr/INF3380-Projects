
# include <stdio.h>
# include <stdlib.h>
# include <omp.h>
# define START_VAL 4

int modify_value(int private_val){
	return private_val * private_val;
}

int main(int argc, char* argv[]){
	int private_val = 0;
	int reduced_val = 0;

	# pragma omp parallel default(none) shared(reduced_val) private(private_val)
	{
		private_val = START_VAL;
		private_val = modify_value(private_val);

		/* Let each thread add its private_val to reduced_val one
		after another (so there is no simultaneous memory access.). */
		# pragma omp critical
		{
			reduced_val += private_val;
		}
	}

	printf("reduced_val = %d\n", reduced_val);

	return(0);
}
