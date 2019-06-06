#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


void multiplicaBlocoOMPIKJ(int size, int n, int **A, int **B, int **C, int TILE, int nt);
int multiplicarOmp(int **mat_a, int **mat_b, int **mat_c, int N, int L, int M, int nt);
int multiplicarOmpIKJ(int **mat_a, int **mat_b, int **mat_c, int N, int L, int M, int nt);
void multiplicaBlocoOMP(int size, int n, int **A, int **B, int **C, int TILE, int nt);
