#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrix_serial(float *A, float *B, float *C, int Am, int Bn)
{
    int i, j, k;
    for (i = 0; i < Am; i++)
    {
        for (j = 0; j < Am; j++)
        {
            C[i*Am + j] = 0;
            for ( k = 0; k< Bn; k++)
            {
                C[i*Am + j] += A[i*Bn + k] * B[j + k*Am];
            }
        }
    }
}


void matrix_parallel(float *A, float *B, float *C, int Am, int Bn, int job_size)
{
    int i, j, k;
    for ( i = 0; i < job_size; i++)
    {
       for( j = 0; j < Am ; j++)
       {
           C[i*Am + j] = 0;
           for(k = 0; k < Bn; k++)
           {
               C[i*Am + j]+=A[i*Bn + k] * B[j + k*Am];
           }
       }
    }
}

void matrix_job_log(float *C, int n, int job_size)
{
    int i,j;
    for(i = 0; i < job_size; i++)
    {
        for(j = 0; j < n; j++)
        {
            printf("%f ",C[i*n + j]);
        }
    }
}


int IsEqual(float *A, float *B, int m, int n)
{
    //return 1 for every eleemnt in A is equal to B
    //return 0 for discrepancy
    int i, j;
    for (i = 0; i < m ; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (A[i*n + j] != B[i*n + j])
            {
                printf("i is %d j is %d A is %f B is %f\n",i,j,A[i*n + j],B[i*n + j]);
                return 0;
            }
        }
    }
    return 1;
}

void log_matrix(float *A, int m, int n)
{
    int i, j;
    for (i = 0; i < m; i++)
    {
        for(j = 0; j< n; j++)
        {
            printf("%f ",A[i*n +j]);
        }
        printf("\n");
    }
}

void matrix_generator (float *A, int m, int n)
{
    int i, j;
//    int cnt = 0;
    srand (time(NULL));
    for (i = 0; i< m; i++)
    {
        for ( j = 0; j < n; j++)
        {
           A[i*n + j] = (double) (rand())/ ((double)RAND_MAX);
//            A[i*n + j] = cnt;
//            cnt++;
        }
    }
}

//testing program for commom.c functions

// int main (int argc, char **argv)
// {
//    int Am = 3;
//    int An = 4;
//    float *A = (float *) malloc(Am*An*sizeof(float));
//    matrix_generator(A, Am, An);
//    log_matrix(A,Am,An);
//    int Bm = 4;
//    int Bn = 3;
//    float *B = (float *) malloc(Bm*Bn*sizeof(float));
//    matrix_generator(B, Bm, Bn);
//    int Cm = Am;
//    int Cn = Bn;
//    float *C = (float *) malloc(Cm*Cn*sizeof(float));
//    float *C_parallel = (float *) malloc(Cm*Cn*sizeof(float));
//    matrix_serial(A,B,C,Am,An);
//    printf("A is \n");
//    log_matrix (A,Am,An);
//    printf("\nB is \n");
//    log_matrix(B,Bm,Bn);
//    printf("\nC is \n");
//    log_matrix(C,Cm,Cn);
//    matrix_parallel(A,B,C_parallel,Am,An,2);
//    matrix_parallel(A+(2*An),B,C_parallel+(2*Cn),Am,An,1);
//    printf("\nC parallel is \n");
//    log_matrix(C_parallel,Cm,Cn);
//    printf("is equal %d\n",IsEqual(C,C_parallel,Cm,Cn));
// }
