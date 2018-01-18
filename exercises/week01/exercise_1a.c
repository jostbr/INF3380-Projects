# include <stdio.h>
# include <math.h>

int main(){
    int i;
    int N = 20;
    double sum = 0.0;

    for (i = 0; i < N; i++){
        sum += pow(-1.0, i) * (1.0 / pow(2.0, 2*i));
        printf("i = %d:\t sum = %0.8f\n", i, sum);
    }

    return(0);
}
