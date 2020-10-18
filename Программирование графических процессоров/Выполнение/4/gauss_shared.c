#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <math.h>
#include <sys/time.h>
#include <cuda_profiler_api.h>
#include <cudaProfiler.h>

#define THREADS_PER_BLOCK 128

#define CUDA_CHECK_RETURN(value) {\
		cudaError_t _m_cudaStat = value;\
		if (_m_cudaStat != cudaSuccess) {\
				fprintf(stderr, "Error %s at line %d in file %s\n",\
				cudaGetErrorString(_m_cudaStat), __LINE__, __FILE__);\
				exit(1);\
		} }

float *init_matrix(int size)
{
	srand(time(NULL));
	float *matrix_ab = (float*)malloc(sizeof(float *) * size * (size + 1));

	for(int col = 0; col < size; col++) {
		for(int row = 0; row < size + 1; row++) {
				matrix_ab[col * size + row] = rand() % 100;//0.1 * (rand() % 100);
		}
	}
	return matrix_ab;
}

__global__ void straight_row_shared(float *AB, int rows, int cols, int equation)
{
	int threadId = threadIdx.x + blockDim.x * blockIdx.x;

	if(threadId >= (rows - 1 - equation) * (cols - equation))
		return;

	 __shared__ float AB_shared[3];

	int el_row = equation + threadId / (cols - equation) + 1;
	int el_col = equation + threadId % (cols - equation);
	int el = el_col + el_row * cols;

	int x_el = el_col + equation * cols;

	int pivot = equation + equation * cols;
	int main2_el = equation + el_row * cols;

	AB_shared[0] = AB[main2_el];
		AB_shared[1] = AB[pivot];
		AB_shared[2] = AB[x_el];

	float lik = AB_shared[0] / AB_shared[1];

	AB[el] -= lik * AB_shared[2];
}

__global__ void return_row_shared(float *AB, int rows, int cols, int equation)
{

	int columns = cols - 2 - equation;
	int start_index = equation * cols + equation + 1;
	int j = columns % 2;

	 __shared__ float AB_shared[4];

	 AB_shared[0] =  AB[start_index + threadIdx.x];

	for(int i = columns / 2; i > 0; i /= 2) {

		if(threadIdx.x >= i)
			return;

		AB_shared[1] = AB[start_index + threadIdx.x + i + j];
		AB[start_index + threadIdx.x] =  AB_shared[0] + AB_shared[1];
		AB[start_index + threadIdx.x + i + j] = 0;
		if(j == 1)
			i++;

		j = i % 2;
		__syncthreads();
	}

	int X = (equation + 1) * cols - 1;
	int diag_el = equation * cols + equation;

	AB_shared[2] = AB[diag_el + 1];

	if(diag_el + 1 != X) {
		AB[X] -= AB_shared[2];
		AB[diag_el + 1] = 0.0;
	}

	AB_shared[3] = AB[diag_el];

	AB[X] /= AB_shared[3];
	AB[diag_el] = 1.0;
}


__global__ void multiple_column(float *AB, int rows, int cols, int row)
{
	AB[(threadIdx.x * cols) + row] *= AB[cols*(row + 1) - 1];
}

int main(int argc, char ** argv)
{
	double start;
	int size = atoi(argv[1]);

	float *AB = init_matrix(size);

	int num_blocks;
	float *device_AB;
	cudaMalloc(&device_AB, sizeof(float)*size*(size + 1));
	cudaMemcpy(device_AB, (void*)AB, sizeof(float)*size*(size + 1), cudaMemcpyHostToDevice);

	//Прямой ход
	for(int column = 0; column < size; column++) {

		num_blocks = ((size - column)*((size + 1) - column) - 1) / THREADS_PER_BLOCK + 1;
		straight_row_shared <<<num_blocks, THREADS_PER_BLOCK>>> (device_AB, size, (size + 1), column);
		cudaDeviceSynchronize();
	}

	//Обратный ход
	for(int row = size - 1; row >= 0; row--) {
		return_row_shared <<<1, size + 1>>> (device_AB, size, size + 1, row);
		multiple_column <<<1, row>>> (device_AB, size, size + 1, row);
		cudaDeviceSynchronize();
	}

	cudaMemcpy(AB, (void*)device_AB, sizeof(float)*size*(size + 1), cudaMemcpyDeviceToHost);
	cudaFree(device_AB);

	return 0;
}