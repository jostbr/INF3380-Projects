# include <stdio.h>
# include <stdlib.h>
# include <time.h>

int main(){
    int i;
    int array_length = 10;
    int arr[array_length];
    int rand_limit = 100;
    int current_value;
    int max_value = 0;
    int min_value = rand_limit;
    srand(time(NULL));

    for (i = 0; i < array_length; i++){
        current_value = rand() % rand_limit;
        arr[i] = current_value;
        printf("Index %d: %d\n", i, arr[i]);

        if (current_value > max_value){
            max_value = current_value;
        }

        else if (current_value < min_value){
            min_value = current_value;
        }
    }

    printf("\n");
    printf("MIN VALUE: %d\n", min_value);
    printf("MAX VALUE: %d\n", max_value);
    return 0;
}
