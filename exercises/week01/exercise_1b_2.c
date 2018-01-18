# include <stdio.h>
# include <stdlib.h>
# include <time.h>

int main(){
    int array_length;

    // Ask user for length of array.
    printf("\nEnter length of array\n");
    scanf("%d", &array_length);
    int *arr = malloc(array_length * sizeof(int));

    int rand_limit = 100;
    int current_value;
    int max_value = 0;
    int min_value = rand_limit;
    int i;

    srand(time(NULL));  // Set seed for rand function.

    for (i = 0; i < array_length; i++){
        current_value = rand() % rand_limit;
        arr[i] = current_value;
        printf("Index %d: %d\n", i, current_value);

        if (current_value > max_value){
            max_value = current_value;
        }

        if (current_value < min_value){
            min_value = current_value;
        }
    }

    printf("\n");
    printf("MIN VALUE: %d\n", min_value);
    printf("MAX VALUE: %d\n", max_value);
    return 0;
}
