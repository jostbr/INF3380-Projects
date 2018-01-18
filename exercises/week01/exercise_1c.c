# include <stdio.h>

int main(){
	int m = 2;
	int n = 10;
	int i, j;
	int arr2D[m][n];
	printf("\n");
	
	for (i = 0; i < m; i++){
		for (j = 0; j < n; j++){
			arr2D[i][j] = i + j;
			printf("%d\t", arr2D[i][j]);
			if (j == n - 1){
				printf("\n");
			}
		}
	}
	return 0;
}

// Does the order of loops matter?
/* The preferred order/structure is the one that
gives you the best spatial locality. See web-page
http://stackoverflow.com/questions/9936132/why-
does-the-order-of-the-loops-affect-performance-
when-iterating-over-a-2d-arra
for further explanation of memory locality. */
