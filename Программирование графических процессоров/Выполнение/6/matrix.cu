#include <cuda.h>
#include <cstdio>
#include <sys/time.h>
#include <iostream>
#include <cmath>

#define ROWS 2500
#define COLS 2500
#define BLOCK_SIZE 32

#define CUDA_CHECK_RETURN(value) {\
	cudaError_t _m_cudaStat = value;\
	if(_m_cudaStat != cudaSuccess) {\
		fprintf(stderr, "Error %s at line %d in file %s\n",\
		cudaGetErrorString(_m_cudaStat), __LINE__, __FILE__);\
		exit(1);\
	} }

double wtime()
{
	struct timeval t;
	gettimeofday(&t,NULL);
	return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void print_matrix(float *a, size_t rows, size_t cols)
{
	for (int i = 0; i < rows * cols; i++) {
		if(i % cols == 0 ) {
			printf("\n");
		}
		printf(" %g", a[i]);
	}
	printf("\n");
}

__global__ void gDgemm(float* a, float *b, float *c, int count_rows, int count_cols)
{
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	if(row < count_rows && col < count_cols) {
		float s = 0.0;
		for (int k = 0; k < COLS; k++) {
			s += a[row * COLS + k] * b[k * ROWS + col];
		}
		c[row * count_cols + col] = s;
	}
}
void init_matrix(float **a, float **b, float **c)
{
	CUDA_CHECK_RETURN(cudaHostAlloc(a, ROWS * COLS * sizeof(float), cudaHostAllocDefault));
	CUDA_CHECK_RETURN(cudaHostAlloc(b, COLS * ROWS * sizeof(float), cudaHostAllocDefault));
	CUDA_CHECK_RETURN(cudaHostAlloc(c, ROWS * ROWS * sizeof(float), cudaHostAllocDefault));

	srand(time(NULL));

	for (int i = 0; i < ROWS * COLS ; i++) {
		(*a)[i] = rand() % 10+1;
		(*b)[i] = rand() % 10+1;
	}
}

void dgemm_with_streams(float *host_a, float *host_b, float *host_c, int streams) {
	float *device_a, *device_b, *device_c;
	int si, count_rows = ceil(ROWS / (float)streams), size_chunk = count_rows * COLS;

	cudaStream_t stream[streams];
	for (int i = 0; i < streams; i++) {
		cudaStreamCreate(&stream[i]);
	}

	CUDA_CHECK_RETURN(cudaMalloc(&device_a, ROWS * COLS * sizeof(float)));
	CUDA_CHECK_RETURN(cudaMalloc(&device_b, COLS * ROWS * sizeof(float)));
	CUDA_CHECK_RETURN(cudaMalloc(&device_c, ROWS * ROWS * sizeof(float)));
	CUDA_CHECK_RETURN(cudaMemcpy(device_b, host_b, COLS * ROWS * sizeof(float), cudaMemcpyHostToDevice));

	dim3 gridDim = dim3(ceil(ROWS / (float)BLOCK_SIZE), ceil(count_rows / (float)BLOCK_SIZE), 1);
	dim3 blockDim = dim3(BLOCK_SIZE, BLOCK_SIZE, 1);

	double time = wtime();
	for (si = 0; si < streams; ++si) {
		float * dev_a_matrix_part = device_a + si * size_chunk;
		float * dev_c_matrix_part = device_c + si * count_rows * ROWS;
		float * host_a_matrix_part = host_a + si * size_chunk;
		float * host_c_matrix_part = host_c + si * count_rows * ROWS;

		if( count_rows * streams > ROWS ) {
			int diff = (count_rows * streams - ROWS);
			CUDA_CHECK_RETURN(cudaMemcpyAsync(dev_a_matrix_part, host_a_matrix_part, (count_rows - diff) * COLS * sizeof(float), cudaMemcpyHostToDevice, stream[si]));
			gDgemm <<< gridDim, blockDim, 0, stream[si]>>> (device_a + si * size_chunk, device_b, device_c + si * count_rows * ROWS, count_rows - diff, ROWS);
			CUDA_CHECK_RETURN(cudaMemcpyAsync(host_c_matrix_part, dev_c_matrix_part, (count_rows - diff) * ROWS * sizeof(float), cudaMemcpyDeviceToHost, stream[si]));
		} else {
			CUDA_CHECK_RETURN(cudaMemcpyAsync(dev_a_matrix_part, host_a_matrix_part, size_chunk * sizeof(float), cudaMemcpyHostToDevice, stream[si]));
			gDgemm <<< gridDim, blockDim,  0, stream[si]>>> (device_a + si * size_chunk, device_b, device_c + si * count_rows * ROWS, count_rows, ROWS);
			CUDA_CHECK_RETURN(cudaMemcpyAsync(host_c_matrix_part, dev_c_matrix_part, count_rows * ROWS * sizeof(float), cudaMemcpyDeviceToHost, stream[si]));
		}
	}

	cudaDeviceSynchronize();

	time = wtime() - time;
	printf("Time: %f ms\n", time);

	//print_matrix(host_c, ROWS, COLS);

	cudaFree(device_a);
	cudaFree(device_b);
	cudaFree(device_c);
}

int main ()
{
	int streams = 1;
	float *host_a = NULL;
	float *host_b = NULL;
	float *host_c = NULL;

	init_matrix(&host_a, &host_b, &host_c);

	printf("Work with %d thread :\n", streams);
	dgemm_with_streams(host_a, host_b, host_c, streams);

	printf("Enter the number of threads to start: ");
	scanf("%d", &streams);
	printf("Work with %d thread :\n", streams);

	dgemm_with_streams(host_a, host_b, host_c, streams);
	printf("\n");

	return 0;
}