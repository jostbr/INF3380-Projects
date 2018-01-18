#include <stdio.h>

int main(){
  int i, j, k;
  int nx = 6, ny = 7, nz = 8;
  int arr[nx][ny][nz];

  for (i = 0; i < nx; i++){
    for (j = 0; j < ny; j++){
        for (k = 0; k < nz; k++){
            arr[i][j][k] = i + j + k;
        }
    }
  }

  printf("%d", arr[nx - 1][ny - 1][nz - 1]);
  return 0;
}
