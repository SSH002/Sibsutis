#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <sys/time.h>

double *a, *b, *c;
int n, count, bs;

//#define hertz 2500000000		//В классе
#define hertz 2300000000		//Дома

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

void dgemm()
{
	int i, j, k;
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			for (k = 0; k < n; ++k) {
				*(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
			}
		}
	}
}

void dgemm_transpose()
{
	int i, j, k;
	for (i = 0; i < n; ++i) {
		for (k = 0; k < n; ++k) {
			for (j = 0; j < n; ++j) {
				*(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
			}
		}
	}
}

void dgemm_block()
{
	int i, j, k, i0, j0, k0;
	double *a0, *b0, *c0;
	for (i = 0; i < n; i += bs) {
		for (j = 0; j < n; j += bs) {
			for (k = 0; k < n; k += bs) {
				for (i0 = 0, c0 = (c + i * n + j), a0 = (a + i * n + k); i0 < bs; ++i0, c0 += n, a0 += n) {
					for (k0 = 0, b0 = (b + k * n + j); k0 < bs; ++k0, b0 += n) {
						for (j0 = 0; j0 < bs; ++j0) {
							c0[j0] += a0[k0] * b0[j0];
						}
					}
				}
			}
		}
	}
}

void print_matrix(double *a, int n)
{
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			printf("%12.2f", *(a + i * n + j));
		}
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	if((argc < 5) || (atoi(argv[1]) == 2) && argc < 6) {
		printf("Ошибка! Введены некорректные данные!");
		return -1;
	}

	int method = atoi(argv[1]);	//вызываемый метод
	n = atoi(argv[2]);		//кол-во элементов
	int max = atoi(argv[3]);	//наибольшее случ. число
	count = atoi(argv[4]);		//кол-во запусков
	bs = atoi(argv[5]);		//размер блока

	a = (double*) malloc(n * n * sizeof(double));
	b = (double*) malloc(n * n * sizeof(double));
	c = (double*) malloc(n * n * sizeof(double));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			a[i * n + j] = rand() % max;
			b[i * n + j] = rand() % max;
			c[i * n + j] = 0;
		}
	}

	uint64_t t;
	switch (method) {
	case 0:
		printf("DGEMM\n");
		t = tsc_wtime();
		for (int i = 0; i < count; ++i) {
			dgemm();
		}
		t = tsc_wtime() - t;
	break;
	case 1:
		printf("DGEMM Transpose\n");
		t = tsc_wtime();
		for (int i = 0; i < count; ++i) {
			dgemm_transpose();
		}
		t = tsc_wtime() - t;
	break;
	case 2:
		printf("DGEMM Block (%d)\n", bs);
		t = tsc_wtime();
		for (int i = 0; i < count; ++i) {
			dgemm_block();
		}
		t = tsc_wtime() - t;
	break;
	}
	t = t / count;

	printf("Elapsed time: %lf sec.\n", (double)t / (double)hertz);
	return 0;
}