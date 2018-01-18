# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# define N 400000

int main(){
    // Create binary file and write in a.
    int i;
    int arr[N];
    for (i = 0; i < N; i++){        // Fill arr with values.
        arr[i] = i;
    }

    FILE *wfb = fopen("binary_file.bin", "wb");
    if (wfb == NULL){
        printf("Can't open binary write file!");
        exit(1);
    }

    for (i = 0; i < N; i++){
        fwrite(&arr[i], sizeof(arr[0]), 1, wfb);
    }

    fclose(wfb);

    // Read back data from the binary file.
    for (i = 0; i < N; i++){        // Reset arr.
        arr[i] = 0;
    }

    FILE *rfb = fopen("binary_file.bin", "rb");
    if (rfb == NULL){
        printf("Can't open binary read file!");
        exit(1);
    }

    for (i = 0; i < N; i++){
        fread(&arr[i], sizeof(arr[i]), 1, rfb);
    }

    fclose(rfb);

    //Print values read from file
    /*for (i = 0; i < N; i++){
        printf("Value written and read is: %d\n", arr[i]);
    }*/

    return 0;
}

/* For some reason there could not be written more than 25-26 values
to the text file. Anyway after running the to .c-files we find that
the binary write/read functions 27% faster. And when tested with
N = 400000 we saw 0.064 s vs 0.085 s for binary and ASCII respectively.*/

