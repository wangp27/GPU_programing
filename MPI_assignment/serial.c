#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main (int argc, char **argv)
{
    int Am = 10000;
    int An = 32;
    int Bm = An;
    int Bn = Am;
    int Cm = Am;
    int Cn = Bn;

    float *A = (float *) malloc(Am*An*sizeof(float));
    float *B = (float *) malloc(Bm*Bn*sizeof(float));
    float *C_serial = (float *) malloc(Cm*Cn*sizeof(float));

    matrix_generator(A, Am, An);
    matrix_generator(B, Bm, Bn);
    matrix_serial(A,B,C_serial,Am,An);

    free(A);
    free(B);
    free(C_serial);

    return 0;
}
