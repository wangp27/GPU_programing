#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "mpi.h"

int main (int argc, char **argv)
{
    int ierr, procid, numprocs;
    int Am = 1000;
    int An = 32;
    int Bm = An;
    int Bn = Am;
    int Cm = Am;
    int Cn = Bn;
    int i;
    int job_load;
    int remain_job;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);

    job_load = (int) Am / numprocs;
    remain_job = Am - job_load*(numprocs - 1);

    float *A_comm = (float *) malloc(job_load*An*sizeof(float));
    float *B_comm = (float *) malloc(Bm*Bn*sizeof(float));
    float *C_comm = (float *) malloc(job_load*Cn*sizeof(float));
    float *A = (float *) malloc(Am*An*sizeof(float));
    float *B = (float *) malloc(Bm*Bn*sizeof(float));
    float *C_parallel = (float *) malloc(Cm*Cn*sizeof(float));
    float *C_serial = (float *) malloc(Cm*Cn*sizeof(float));

    for(i = 0; i < Cm*Cn; i++)
    {
        *(C_comm + i) = 0;
        *(C_parallel + i) = 0;
    }

    if(procid == 0)
    {
        matrix_generator(A, Am, An);
        matrix_generator(B, Bm, Bn);

        MPI_Scatter(A, job_load*An/2, MPI_DOUBLE, A_comm, job_load*An/2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(B,Bm*Bn,MPI_FLOAT,0,MPI_COMM_WORLD);

        matrix_parallel(A_comm,B,C_comm,Am,An,job_load);

        MPI_Gather(C_comm,job_load*Cn,MPI_FLOAT,C_parallel,job_load*Cn/2,MPI_DOUBLE,0,MPI_COMM_WORLD);
        matrix_serial(A,B,C_serial,Am,An);
        if(IsEqual(C_serial,C_parallel,Cm,Cn))
        {
            printf("serial parallel check pass \n");
        }
        else
        {
            printf("serial parallel check failed\n");
        }

    }
    else
    {
        MPI_Scatter(A, job_load*An/2, MPI_DOUBLE, A_comm, job_load*An/2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(B,Bm*Bn,MPI_FLOAT,0,MPI_COMM_WORLD);
        matrix_parallel(A_comm,B,C_comm,Am,An,job_load);

        MPI_Gather(C_comm,job_load*Cn,MPI_FLOAT,C_parallel,job_load*Cn/2,MPI_DOUBLE,0,MPI_COMM_WORLD);
    }
    free(A_comm);
    free(B_comm);
    free(C_comm);
    free(A);
    free(B);
    free(C_serial);
    MPI_Finalize();
    return 0;
}
