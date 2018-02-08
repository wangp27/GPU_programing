#ifndef COMMON_H
#define COMMON_H
void matrix_serial(float *A, float *B, float *C, int Am, int Bn);
void matrix_parallel(float *A, float *B, float *C, int Am, int Bn, int job_size);
void matrix_job_log(float *C, int Am, int job_size);
int IsEqual(float *A, float *B, int m, int n);
void log_matrix(float *A, int m, int n);
void matrix_generator(float *A, int m, int n);


#endif
