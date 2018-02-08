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
    int job_load;
    int remain_job;
    int i;


    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);
    job_load = (int) Am / numprocs;
    remain_job = Am-job_load*(numprocs-1);
    MPI_Request *Areqs = (MPI_Request *)malloc((numprocs-1)*sizeof(MPI_Request));
    MPI_Request *Breqs = (MPI_Request *)malloc((numprocs-1)*sizeof(MPI_Request));
    MPI_Request *Creqs = (MPI_Request *)malloc((numprocs-1)*sizeof(MPI_Request));
    MPI_Status *stats = (MPI_Status *)malloc((numprocs-1)*sizeof(MPI_Status));
    float *A_comm = (float *) malloc(Am*An*sizeof(float));
    float *A = (float *) malloc(Am*An*sizeof(float));
    float *B_comm = (float *) malloc(Bm*Bn*sizeof(float));
    float *B = (float *) malloc(Bm*Bn*sizeof(float));
    float *C_comm = (float *) malloc(Cm*Cn*sizeof(float));
    float *C_serial = (float *) malloc(Cm*Cn*sizeof(float));
    for (i = 0; i < Cm*Cn; i++)
    {
        *(C_comm + i) = 0;
    }

    if(procid == 0)
    {
        matrix_generator(A, Am, An);
        matrix_generator(B, Bm, Bn);
        for(i = 1; i<numprocs-1;i++)
        {
            MPI_Isend(A+(job_load*i*An),job_load*An,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&Areqs[i-1]);
            MPI_Isend(B,Bm*Bn,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&Breqs[i-1]);
        }
        MPI_Isend(A+(job_load*(numprocs-1)*An),remain_job*An,MPI_DOUBLE,numprocs-1,0,MPI_COMM_WORLD,&Areqs[numprocs-2]);
        MPI_Isend(B,Bm*Bn,MPI_DOUBLE,numprocs-1,0,MPI_COMM_WORLD,&Breqs[numprocs-2]);

        matrix_parallel(A,B,C_comm,Am,An,job_load);

       MPI_Waitall(numprocs-1,Areqs,stats);
       MPI_Waitall(numprocs-1,Breqs,stats);

        for(i=1;i<numprocs-1;i++)
        {
            MPI_Recv(C_comm + (job_load*i*Cn),job_load*Cn,MPI_DOUBLE,i,0,MPI_COMM_WORLD,NULL);
        }
        MPI_Recv(C_comm + job_load*(numprocs-1)*Cn,remain_job*Cn,MPI_DOUBLE,numprocs-1,0,MPI_COMM_WORLD,NULL);
        matrix_serial(A,B,C_serial,Am,An);

        if(IsEqual(C_serial,C_comm,Cm,Cn))
        {
            printf("serial parallel check pass\n");
        }
        else
        {
            printf("serial parallel check failed\n");
        }
    }
    else
    {
        if(procid != numprocs-1)
        {
            MPI_Recv(A_comm + (job_load * procid * An),remain_job*An,MPI_DOUBLE,0,0,MPI_COMM_WORLD,NULL);
            MPI_Recv(B_comm,Bm*Bn,MPI_DOUBLE,0,0,MPI_COMM_WORLD,NULL);
            matrix_parallel(A_comm+(job_load*procid*An),B_comm,C_comm+(job_load*procid*Cn),Am,An,job_load);
            MPI_Isend(C_comm + (job_load*procid*Cn), job_load*Cn, MPI_DOUBLE, 0, 0,MPI_COMM_WORLD,Creqs);
        }
        else
        {
            MPI_Recv(A_comm + (job_load * procid * An),remain_job*An,MPI_DOUBLE,0,0,MPI_COMM_WORLD,NULL);
            MPI_Recv(B_comm,Bm*Bn,MPI_DOUBLE,0,0,MPI_COMM_WORLD,NULL);
            matrix_parallel(A_comm + (job_load*procid*An),B_comm,C_comm+(job_load*procid*Cn),Am,An,remain_job);
            MPI_Isend(C_comm + (job_load*procid*Cn),remain_job*Cn,MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,Creqs);
        }
    }
    free(A);
    free(B);
    free(stats);
    free(A_comm);
    free(B_comm);
    free(C_serial);
    free(C_comm);
    free(Areqs);
    free(Breqs);
    free(Creqs);
    MPI_Finalize();
}
