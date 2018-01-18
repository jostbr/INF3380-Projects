# include <stdio.h>
# define ROWS 1080
# define COLUMNS 1920

int main(){
    /* Sketch of a function that smooths an
    image of ROWSxCOLUMNS pixels at pixel v[i][j].*/
    int i, j;
    float c = 1.0;
    float v[ROWS][COLUMNS];     // Here the image should be loaded.
    for (i = 0; i < M; i++){
        for (j = 0; j < N; j++){
            v[i][j] += c * (v[i-1][j] + v[i][j-1] - 4v[i][j+1]+v[i+1][j]);

        }
    }
}
