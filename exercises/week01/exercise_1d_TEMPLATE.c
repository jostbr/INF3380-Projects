#include <stdio.h>
#include <stdlib.h>

int main (int nargs, char** args)
{
  int i,j,k;
  int nz=8, ny=7, nx=6;

  double* base_array = (double*)malloc(nz*ny*nx*sizeof(double));

  double*** array3d = (double***)malloc(nz*sizeof(double**));

  for (k=0; k<nz; k++){
    array3d[k] =(double**)malloc(ny*sizeof(double*));
  }
  for (k=0; k<nz; k++){
    for (j=0; j<ny; j++){
      array3d[k][j] = &(base_array[k*ny*nx+j*ny]);
    }
  }
  for (k=0; k<nz; k++){
    for (j=0; j<ny; j++){
      for (i=0; i<nx; i++){
        array3d[k][j][i] = k*(ny*nx)+j*nx+i;
      }
    }
  }
  for (k=0; k<nz; k++){
    free(array3d[k]);
    free(array3d);
    free(base_array);
  }

  return 0;
}
