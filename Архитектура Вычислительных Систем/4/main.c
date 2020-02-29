#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>
//#define hertz 2500000000		//В классе
#define hertz 2300000000		//Дома

uint64_t tsc_wtine();
double rand_double(int max);
double saxpy_blas(const int N, const int a);
double dgemm_blas(const int N);

//Замеры времени
uint64_t tsc_wtime()
{
	uint32_t high, low;

	__asm__ __volatile__ (
	"rdtsc\n"
	"movl %%edx, %0\n"
	"movl %%eax, %1\n"
		: "=r" (high), "=r" (low)
		:: "%rax", "%rbx", "%rcx", "%rdx"
	);

	return ((uint64_t)high << 32) | low;
}

double rand_double(int max)
{
	srand(time(0));
	double remainder = (rand() % max) / RAND_MAX;
	remainder = remainder - floor(remainder);

	int number = rand() % max;

	return (double)number + remainder;
}

//Умножение векторов
double saxpy_blas(const int N, const int a)
{
	uint64_t tbuf;
	int *vector = (int*) malloc(sizeof(int) * N);
	int *X = (int*) malloc(sizeof(int) * N);
	int *Y = (int*) malloc(sizeof(int) * N);

	unsigned int i, j;

	srand(time(NULL));

	//Заполнение векторов
	for (i = 0; i < N; ++i) {
		X[i] = rand() % 1000;
		Y[i] = rand() % 1000;
	}

	tbuf = tsc_wtime();

	//Перемножение векторов
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			vector[i] += a * X[i] + Y[j];
		}
	}

	free(vector);
	free(X);
	free(Y);

	tbuf = tsc_wtime() - tbuf;
	return (double)tbuf / (double)hertz;
}

//Умножение матриц
double dgemm_blas(const int N)
{
	double time;
	uint64_t tbuf;
	double **Matr = (double**) malloc(sizeof(double) * N);
 	double **A = (double**) malloc(sizeof(double) * N);
 	double **B = (double**) malloc(sizeof(double) * N);
	unsigned int i, j, k;

	//Заполнение матриц
	for (i = 0; i < N; ++i) {
		Matr[i] = (double*) malloc(sizeof(double) * N);
		A[i] = (double*) malloc(sizeof(double) * N);
		B[i] = (double*) malloc(sizeof(double) * N);
		for (j = 0; j < N; ++j) {
			A[i][j] = rand_double(1000);
			B[i][j] = rand_double(1000);
		}
	}

	tbuf = tsc_wtime();

	//Перемножение матриц
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			for (k = 0; k < N; ++k) {
				Matr[i][k] = A[i][j] * B[j][k];
			}
		}
	}

	for (i = 0; i < N; ++i) {
		free(Matr[i]);
		free(A[i]);
		free(B[i]);
	}
	free(Matr);
	free(A);
	free(B);

	tbuf = tsc_wtime() - tbuf;
	return (double)tbuf / (double)hertz;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("\nОшибка! Невенрные аргумент. Пример: ./lb4 <N> <a>;");
		printf("\nгде N - кол-во элементов, a - константа.\n");
		return 1;
	}

	printf("\n");
	double time1 = saxpy_blas(atoi(argv[1]), atoi(argv[2]));
	double time2 = dgemm_blas(atoi(argv[1]));

	printf("SAXPY_BLASS: %lf sec, DGEMM_BLASS: %lf sec\n", time1, time2);

	return 0;
}