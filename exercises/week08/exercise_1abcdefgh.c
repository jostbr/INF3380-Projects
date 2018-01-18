
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <omp.h>
# define N ((int) 4e6)

double foo(double x){
	return cos(sin(x));
}

int main(int argc, char* argv[]){
	/* ------------------ SETUP BEFFORE EXERCISES ------------------ */
	int i;
	double x = N;

	double* a = (double*)malloc(N * sizeof(double));
	double* b = (double*)malloc(N * sizeof(double));

	/* Checking if malloc succeded. */
	if (a == NULL || b == NULL) {
    	perror("malloc");
    	exit(EXIT_FAILURE);
    }

	/* Fill arrays with values. */
	# pragma omp parallel for default(shared) private(i)
	for (i = 0; i < N; i++){
		a[i] = 2.0 * i;
		b[i] = a[i] - 1;
	}
	/* --------------- DONE SETUP BEFFORE EXERCISES --------------- */

	/* Exercise 1a. */
	# pragma omp parallel for default(shared) private(i)
	for (i = 0; i < (int)sqrt(x); i++){
		a[i] = 2.3 * x;

		if (i < 10){
			b[i] = a[i];
		}
	}

	/* Exercise 1b. */
	/* The control expression in the for loop is non-ordinary and
	will cause unpredictable stops in the loop. Therefor this loop
	is not well suited for OpenMP pragam parallelization since we can
	not share an unknown amount of iterations (the flag will be FALSE
	at unpredictable times depending on the values in a and b). */
	int flag = 0, n = N;

	for (i = 0; (i < n) && (!flag); i++){
		a[i] = 2.3 * x;

		if (a[i] = b[i]){
			flag = 1;
		}
	}

	/* Exercise 1c. */
	# pragma omp parallel for default(shared) private(i)
	for (i = 0; i < n; i++){
		a[i] = foo((double)i);
	}

	/* Exercise 1d. */
	# pragma omp parallel for default(shared) private(i)
	for (i = 0; i < n; i++){
		a[i] = foo((double)i);

		if (a[i] < b[i]){
			a[i] = b[i];
		}
	}

	/* Exercise 1e. */
	/* Again e get and unpredictable amount of loop iterations so
	we cannot simply parallelize by dividing the iterations among
	threads. Hence OpenMP pragma does not work here. */
	for (i = 0; i < n; i++){
		a[i] = foo((double)i);

		if (a[i] < b[i]){
			break;
		}
	}

	/* Exercise 1f. */
	double dotp = 0;
	# pragma omp parallel for default(none) private(i) shared(a, b, n) reduction(+:dotp)
	for (i = 0; i < n; i++){
		dotp += a[i] * b[i];
	}

	/* Exercise 1g. */
	/* Cannot parallelize because, to calculate each element in a, we
	need the previous element. So the different iterations of the loop
	are dependent on eachother. Therefore we cannot not parallelize.
	The solution here says that it can, but I don't get this. Okey,
	no I get it; try with different values for k and num_threads and
	see that one thread never has to use a value computed by another. */
	int k = (int)(N / 2);

	# pragma omp parallel for
	for (i = k; i < 2*k; i++){
		a[i] = a[i] + a[i - k];
	}

	/* Exercise 1h. */
	/* Quite similar to 1g), but here (beacuse of the upper bound on i)
	we get memory issues if n >= 2*k. We the use an if "clause". */
	double bb = 0.23;
	k = k - 1;

	# pragma omp parallel for if (n < 2*k)
	for (i = k; i < n; i++){
		a[i] = bb * a[i - k];
	}

	free(a);
	free(b);

	return(0);
}
