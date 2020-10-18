#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform.h>
#include <thrust/fill.h>
#include <thrust/sequence.h>
#include <thrust/iterator/zip_iterator.h>
#include <thrust/tuple.h>
#include <thrust/extrema.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cublas_v2.h>
#include <cuda.h>
#include <curand.h>

//Заполнение матрицы псевдослучайным числами
void fill_rand(float *arr, int rows, int cols)
{
	//Создание генератора псевдослучайных чисел
	curandGenerator_t rng;
	curandCreateGenerator(&rng, CURAND_RNG_PSEUDO_DEFAULT);

	//Инициализация
	curandSetPseudoRandomGeneratorSeed(rng, (unsigned long long) clock());

	//Заполнение матрицы
	curandGenerateUniform(rng, arr, rows * cols);
}

//Перемножение матриц
void mmul(const float *arrA, const float *arrB, float *result, const int m, const int k, const int n)
{
	int lda = m, ldb = k, ldc = m;
	const float alf = 1;
	const float bet = 0;
	const float *alpha = &alf;
	const float *beta = &bet;

	//Создание дескриптора для сuBLAS
	cublasHandle_t handle;
	cublasCreate(&handle);

	//Непосредственно умножение
	cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, k, alpha, arrA, lda, arrB, ldb, beta, result, ldc);

	//Удаление дескриптора
	cublasDestroy(handle);
}

void print_matrix(const float *arr, int rows, int cols)
{
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			printf(" %f ", arr[j * rows + i]);
		}
		printf("\n");
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	int rows, cols;
	rows = atoi(argv[1]);
	cols = atoi(argv[1]);

	printf("Matrix's size = %d\n", atoi(argv[1]));
	thrust::device_vector<float> d_A(rows * cols), d_B(rows * cols), d_C(rows * cols);

	//Заполнение матрицы А
	fill_rand(thrust::raw_pointer_cast(&d_A[0]), rows, cols);
	//Заполнение матрицы В
	fill_rand(thrust::raw_pointer_cast(&d_B[0]), rows, cols);

	thrust::host_vector<float> h_A = d_A;
	thrust::host_vector<float> h_B = d_B;
	//Вывод маассивов перед умножением
	printf("A = ");
	print_matrix(thrust::raw_pointer_cast(&h_A[0]), rows, cols);
	printf("B = ");
	print_matrix(thrust::raw_pointer_cast(&h_B[0]), rows, cols);

	//Перемножение матриц
	mmul(thrust::raw_pointer_cast(&d_A[0]), thrust::raw_pointer_cast(&d_B[0]), thrust::raw_pointer_cast(&d_C[0]), rows, cols, cols);

	thrust::host_vector<float> h_C = d_C;

	//Вывод результатов
	printf("C = ");
	print_matrix(thrust::raw_pointer_cast(&h_C[0]), rows, cols);

	//Поиск максимального значения
	thrust::device_vector<float>::iterator iter = thrust::max_element(d_C.begin(), d_C.end());
	float max_val = *iter;
	printf("Max element = %f\n", max_val);

	//Поиск минимального значения
	thrust::device_vector<float>::iterator iter2 = thrust::min_element(d_C.begin(), d_C.end());
	float min_val = *iter2;
	printf("Min element = %f\n", min_val);

	return 0;
}