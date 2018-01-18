# include <stdio.h>

int *create_subarray(int arr[], int start, int end){
    /* Function that takes an array arr and returns
    a subarray of a between index start and end. */
    int i;
    static int subarray[4]; // (end - start) + 1

    for (i = 0; i <= end - start; i++){
        subarray[i] = arr[start + i];
    }

    return subarray;
}

int main(){
    /* Main function that illustrates how to use the
    create_subarray() function from above. */
	int i, start = 5, end = 8;
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int *subarray_pointer = create_subarray(a, start, end);

    if (start < 0 || (end > (sizeof(a) / sizeof(a[0])) - 1)){
        printf("Selected start or end indices are out of bounds\n");
        exit(1);
    }

    for (i = 0; i <= (end - start); i++){
        printf("subarray[%d] = %d\n", i, subarray_pointer[i]);
    }

    return 0;
}



