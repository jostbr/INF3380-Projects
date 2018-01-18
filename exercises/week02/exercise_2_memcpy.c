# include <stdio.h>
# include <time.h>
# include <string.h>

int main(){
    /* Function comparing performance differences between
    hand-coded copying of arrays with the built in memcpy func.
    This part tests the built-in memcpy function.*/
    int i, N = 200000;
    float a[N], b[N];
    clock_t clocks_begin, clocks_end;

    for (i = 0; i < N; i++){
        a[i] = i + 0.1;
    }


    clocks_begin = clock();
    memcpy(b, a, sizeof(a));
    clocks_end = clock();

    double time_memcpy = (double)(clocks_end - clocks_begin) / CLOCKS_PER_SEC;
    printf("Time consumed with memcpy copying: %f seconds.\n", time_memcpy);

    return 0;
}

/* We see that that built in memcpy function goes significantly
faster. This might be due to the fact that this manipulates pointers
in stead of actually copying all the elements of the array to the other array.
Time of the manual seem to be around 0.019s and time for memcpy seems to be
be around 0.014s. This is approximately 27% faster which could be significant
when processing larger amounts of data. For some reason this program would
crash if I chose to many elements in the arrays.*/
