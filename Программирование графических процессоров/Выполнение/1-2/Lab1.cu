#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>

__global__ void gTest(float *a, float *b)
{
	a[threadIdx.x + blockDim.x * blockIdx.x] = (float)(threadIdx.x + blockDim.x * blockIdx.x);
	b[threadIdx.x + blockDim.x * blockIdx.x] = 2.0;
	a[threadIdx.x + blockDim.x * blockIdx.x] += b[threadIdx.x + blockDim.x * blockIdx.x];
}

int main(int argc, char *argv[])
{
	float *vec, *vec2, *result;
	int size = 8388608;
	int threads_per_block = atoi(argv[1]);
	int num_of_blocks = (int)(size / threads_per_block);

	result = (float*) malloc(size * sizeof(float));
	cudaMalloc((void**)&vec, size * sizeof(float));
	cudaMalloc((void**)&vec2, size * sizeof(float));
	gTest <<< dim3(num_of_blocks), dim3(threads_per_block) >>> (vec, vec2);

	cudaDeviceSynchronize();
	cudaMemcpy(result, vec, size * sizeof(float), cudaMemcpyDeviceToHost);

	free(result);
	cudaFree(vec);
	cudaFree(vec2);

	return 0;
}