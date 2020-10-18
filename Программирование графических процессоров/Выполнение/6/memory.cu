#include <stdio.h>
#include <cuda.h>

#define SIZE (1024 * 1024)

#define CUDA_CHECK_RETURN(value) {\
	cudaError_t _m_cudaStat = value;\
	if (_m_cudaStat != cudaSuccess) {\
		fprintf(stderr, "Error %s at line %d in file %s\n",\
		cudaGetErrorString(_m_cudaStat), __LINE__, __FILE__);\
		exit(1);\
	} }

float cuda_pageable(int size, bool flag) {
	int *host_a, *dev_a;
	float elapsedTime;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	host_a = (int*)malloc(size * sizeof(int));
	CUDA_CHECK_RETURN(cudaMalloc((void**)&dev_a, size * sizeof(float)));

	cudaEventRecord(start, 0);

	if(flag)
		CUDA_CHECK_RETURN(cudaMemcpy(dev_a, host_a, size * sizeof(float), cudaMemcpyHostToDevice));

	if(!flag)
		CUDA_CHECK_RETURN(cudaMemcpy(host_a, dev_a, size * sizeof(float), cudaMemcpyDeviceToHost));

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);

	free(host_a);
	cudaFree(dev_a);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	return elapsedTime;
}

float cuda_page_locked(int size, bool flag) {
	int *host_a, *dev_a;
	float elapsedTime;
	cudaEvent_t start, stop;

	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	CUDA_CHECK_RETURN(cudaHostAlloc((void**)&host_a, size * sizeof(float), cudaHostAllocDefault));
	CUDA_CHECK_RETURN(cudaMalloc((void**)&dev_a, size * sizeof(float)));
	CUDA_CHECK_RETURN(cudaEventRecord(start, 0));

	if(flag)
		CUDA_CHECK_RETURN(cudaMemcpy(dev_a, host_a, size * sizeof(float), cudaMemcpyHostToDevice));

	if(!flag)
		CUDA_CHECK_RETURN(cudaMemcpy(host_a, dev_a, size * sizeof(float), cudaMemcpyDeviceToHost));

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);
	cudaFreeHost(host_a);
	cudaFree(dev_a);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	return elapsedTime;
}

int main(int argc, char *argv[])
{
	float elapsedTime;
	int numbMB = atoi(argv[1]);

	int size = SIZE * numbMB;

	elapsedTime = cuda_pageable(size, true);
	printf("\nPageable memory:\n\n[CPU -> GPU] = %f ms\n", elapsedTime);

	elapsedTime = cuda_pageable(size, false);
	printf("[GPU -> CPU] =  %f ms\n", elapsedTime);

	elapsedTime = cuda_page_locked(size, true);
	printf("\nPage-locked memory:\n\n[CPU -> GPU] = %f ms\n", elapsedTime);

	elapsedTime = cuda_page_locked(size, false);
	printf("[GPU -> CPU] = %f ms\n\n", elapsedTime);

	return 0;
}