#include <cuda.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define Thread 32

//Метод простых итераций
__global__ void iterative_shared(float *deviceA, float *deviceF, float *deviceB, float *deviceB1, int N)
{
	float temp;

	int i = blockIdx.x * blockDim.x + threadIdx.x;
	deviceB1[i] = deviceF[i];
	for(int j = 0; j < N; j++) {
		if(i != j) {
			deviceB1[i] -= deviceA[j + i * N] * deviceB[j];
		} else {
			temp = deviceA[j + i * N];
		}
	}
	deviceB1[i] /= temp;
}

//Расчёт условия для прекращения вычислений
__global__ void out_of_cycle(float *deviceB, float *deviceB1, float *delta, int N)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	delta[i] += fabs(deviceB[i] - deviceB1[i]);
	deviceB[i] = deviceB1[i];
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		printf("Не задан размер матрицы.");
		return -1;
	}

	srand(time(NULL));
	float *A, *buffer, *B, *B1, *F, *Delta;
	float sum, eps;
	float EPS = 1.e-5;
	int N = atoi(argv[1]);
	float size = N * N;
	int count;
	int Block = (int)ceil((float)N / Thread);

	dim3 Blocks(Block);
	dim3 Threads(Thread);

	int Num_diag = 0.5f * (int)N * 0.3f;

	unsigned int mem_sizeA = sizeof(float) * size;
	unsigned int mem_sizeX = sizeof(float) * (N);

	A = (float*) malloc(mem_sizeA);
	F = (float*) malloc(mem_sizeX);
	buffer = (float*) malloc(mem_sizeX);
	B = (float*) malloc(mem_sizeX);
	B1 = (float*) malloc(mem_sizeX);
	Delta = (float*) malloc(mem_sizeX);

	for(int i = 0; i < size; i++) {
		A[i] = 0.0f;
	}

	for(int i = 0; i < N; i++) {
		A[i + i * N] = rand() % 50 + 1.0f * N;
	}

	for(int k = 1; k < Num_diag + 1; k++) {
		for(int i = 0; i < N - k; i++) {
			A[i + k + i * N] = rand() % 5;
			A[i + (i + k) * N] = rand() % 5;
		}
	}

	for(int i = 0; i < N; ++i) {
		buffer[i] = rand() % 50;
		B[i] = 1.0f;
		B1[i] = 1.0f;
		Delta[i] = 0.0f;
	}

	for(int i = 0; i < N; ++i) {
		sum = 0.0f;
		for(int j = 0; j < N; ++j) {
			sum += A[j + i * N] * buffer[j];
		}
		F[i] = sum;
	}

	//Videlenie pamyatina GPU
	//float *deviceA, *deviceB, *deviceB1, *deviceF, *delta;
	float *deviceA, *deviceF;
	__shared__ float *deviceB, *deviceB1, *delta;

	cudaMalloc((void**)&deviceA, mem_sizeA);
	cudaMalloc((void**)&deviceF, mem_sizeX);
	cudaMalloc((void**)&deviceB, mem_sizeX);
	cudaMalloc((void**)&deviceB1, mem_sizeX);
	cudaMalloc((void**)&delta, mem_sizeX);

	cudaMemcpy(deviceA, A, mem_sizeA, cudaMemcpyHostToDevice);
	cudaMemcpy(deviceF, F, mem_sizeX, cudaMemcpyHostToDevice);
	//cudaMemcpy(deviceB, B, mem_sizeX, cudaMemcpyHostToDevice);
	//cudaMemcpy(deviceB1, B1, mem_sizeX, cudaMemcpyHostToDevice);
	count = 0;
	eps = 1.0f;

	while(eps > EPS) {
		++count;
		cudaMemcpy(delta, Delta, mem_sizeX, cudaMemcpyHostToDevice);
		iterative_shared <<<Blocks, Threads>>> (deviceA, deviceF, deviceB, deviceB1, N);
		out_of_cycle <<<Blocks, Threads>>> (deviceB, deviceB1, delta, N);
		cudaMemcpy(Delta, delta, mem_sizeX, cudaMemcpyDeviceToHost);
		eps = 0.0f;
		for (int j = 0; j < N; j++) {
			eps += Delta[j];
			Delta[j] = 0;
		}
		eps = eps / N;
	}

	cudaMemcpy(B1, deviceB1, mem_sizeX, cudaMemcpyDeviceToHost);
	/*cout<< "Matrix A." <<endl;

	for (inti = 0; i< N; i++) {
		for (int j = 0; j < N; j++) {
			cout << A[i * N + j] << " ";
		}
		cout << endl;
	}

	cout << "Matrix F." <<endl;
	for (int i = 0; i< N; i++) {
	cout << F[i] << " ";
	}
	cout << endl;
	cout << "Matrix X." <<endl;

	for (int i = 0; i< N; i++) {
	cout << buffer[i] << " ";
	}

	cout << endl;
	cout << "Result matrix." <<endl;
	for (inti = 0; i< N; i++) {
	cout << B1[i] << " ";
	}
	cout<<endl;
	*/

	cudaFree(deviceA);
	cudaFree(deviceF);
	cudaFree(deviceB);
	cudaFree(deviceB1);

	free(A);
	free(F);
	free(B);
	free(B1);
	free(buffer);
	free(Delta);

	return 0;
}