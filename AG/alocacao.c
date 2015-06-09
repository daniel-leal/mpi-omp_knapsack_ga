/*
    funcoes especiais p/ inicializacao
    do programa e abstração dos ponteiros

    [ alocacao dinamica de arrays do tipo int ]
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define array1d(x,s)     int *x   = calloc(s+1,sizeof(int))
#define array2d(x,i,j)   int **x  = falloc2d(0,i,0,j)
// #define array3d(x,i,j,k) int ***x = falloc3d(0,i,0,j,0,k)


int **falloc2d(int nrl, int nrh, int ncl, int nch)
/* Aloca dinamicamente Array int 2D na faixa [nrl..nrh][ncl..nch]*/
{
    int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    int **m;

    m=(int **) malloc((size_t)((nrow+1)*sizeof(int*)));

    if (!m) printf("Falha de alocacao!\n");

    m += 1;
    m -= nrl;

    m[nrl]=(int *) malloc((size_t)((nrow*ncol+1)*sizeof(int)));

    if (!m[nrl]) printf("Falha de alocacao!\n");

    m[nrl] += 1;
    m[nrl] -= ncl;

    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

  return m;
}


// int ***falloc3d(int nrl, int nrh, int ncl, int nch, int ndl, int ndh)
// /* Aloca dinamicamente Array int 3D na faixa [nrl..nrh][ncl..nch][ndl..ndh] */
// {
//     int i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1;

//     int ***t;

//     t=(int ***) malloc((size_t)((nrow+1)*sizeof(int**)));

//     if (!t) printf("Falha de alocacao!\n");

//     t += 1;
//     t -= nrl;

//     t[nrl]=(int **) malloc((size_t)((nrow*ncol+1)*sizeof(int*)));

//     if (!t[nrl]) printf("Falha de alocacao!\n");

//     t[nrl] += 1;
//     t[nrl] -= ncl;

//     t[nrl][ncl]=(int *) malloc((size_t)((nrow*ncol*ndep+1)*sizeof(int)));

//     if (!t[nrl][ncl]) printf("Falha de alocacao 3\n");

//     t[nrl][ncl] += 1;
//     t[nrl][ncl] -= ndl;

//     for(j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;

//     for(i=nrl+1;i<=nrh;i++){
//      t[i]=t[i-1]+ncol;
//      t[i][ncl]=t[i-1][ncl]+ncol*ndep;
//      for(j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
//     }

//   return t;
// }

