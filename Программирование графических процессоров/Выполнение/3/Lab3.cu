#include <stdio.h>
#include <cuda.h>
#include <math.h>
#include <stdlib.h>

#define BLOCK_SIZE 32

//����� �������
void show(float* a, int n)
{
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            fprintf(stdout, "%g\t", a[j + i * n]);
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n\n\n");
}

__global__ void matMult_global(float *a, float *b, int n, float* c)
{
    int i = threadIdx.x + blockIdx.x * blockDim.x; //���� ����� -> ���� ������� �������
    int j = threadIdx.y + blockIdx.y * blockDim.y;

    float sum = 0.0f;
    for (int k = 0; k < n; ++k)
        sum += a[i * n + k] * b[k * n + j];
    c[i * n + j] = sum;

}

__global__ void matMult_shared(float *a, float *b, int n, float* c)
{
    int aBegin = n * BLOCK_SIZE * blockIdx.y; //������ ������ ������ ���������� �, ������� ������������ ����
    int aEnd = aBegin + n - 1; //������ ����� ���������� �, ������� ������������ ����
    int aStep = BLOCK_SIZE; //��� ��� �������� ������ �

    int bBegin = BLOCK_SIZE * blockIdx.x; //������ ������ ������ ���������� �, ������� ������������ ����
    int bStep = BLOCK_SIZE * n; //��� ��� �������� �������� B

    float sum = 0.0f; //���������� �������� ����������

    for (int sub_A = aBegin, sub_B = bBegin; sub_A <= aEnd; sub_A += aStep, sub_B += bStep) {
        //��������� ���������� ������ ��� ���������
        __shared__ float buffer_A[BLOCK_SIZE][BLOCK_SIZE];
        __shared__ float buffer_B[BLOCK_SIZE][BLOCK_SIZE];

        //�������� ��������� � � � �� ���������� ������ � �����������
		buffer_A[threadIdx.y][threadIdx.x] = a[sub_A + n * threadIdx.y + threadIdx.x];
        buffer_B[threadIdx.y][threadIdx.x] = b[sub_B + n * threadIdx.y + threadIdx.x];

        __syncthreads(); // ��������, ��� ���������� ��������� ���������

        //������������ ���� ������
        for (int k = 0; k < BLOCK_SIZE; k++)
            sum += buffer_A[threadIdx.x][k] * buffer_B[k][threadIdx.x];
        __syncthreads(); // ��������, ��� ���������� ������ ������ �� �����
    }

    int cBegin = n * BLOCK_SIZE * blockIdx.x + BLOCK_SIZE * blockIdx.x; //������ ��������������� �������� � ���������� ������
    c[cBegin + n * threadIdx.y + threadIdx.x] = sum; //������ �������� � ���������� ������
}

int main (int argc, char *  argv [])
{
    //���� ����������� �������
    if(argc < 2){
        fprintf(stderr, "������� ������ �������: ");
        return -1;
    }

    //�������� ������� ������
    int N = atoi(argv[1]);
	if(N % BLOCK_SIZE) {
        fprintf(stderr, "�������� ������\n");
        return -1;
    }

    int numBytes = N * N * sizeof(float);

    //������������� ��������� �������
    //��������� ������ �� host
    float *a = (float*)malloc(numBytes); //������� �
    float *b = (float*)malloc(numBytes); //������� B
    float *c = (float*)malloc(numBytes); //������� �

    //���������� ������
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            int k = N * i + j;
            a[k] = 2.0f;
            b[k] = 1.0f;
        }

    float *adevice, *bdevice, *cdevice;

    //��������� ������ �� GPU
    cudaMalloc((void**)&adevice, numBytes);
	cudaMalloc((void**)&bdevice, numBytes);
    cudaMalloc((void**)&cdevice, numBytes);


    //������� ����� ����� � ������
    dim3 threads(BLOCK_SIZE, BLOCK_SIZE);
    dim3 blocks(N / threads.x, N / threads.y);

    //����������� ������ � � B c host �� device
    //������������� ���������� �� CPU -> �������� �� GPU
    cudaMemcpy(adevice, a, numBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(bdevice, b, numBytes, cudaMemcpyHostToDevice);

    //������ �������
    matMult_global<<<blocks, threads>>>(adevice, bdevice, N, cdevice);
    cudaDeviceSynchronize();

    //�����������, ����������� ������� � � device �� host
    //���������� ������� -> ���������
    cudaMemcpy(c, cdevice, numBytes, cudaMemcpyDeviceToHost);

	cudaMemcpy (adevice, a, numBytes, cudaMemcpyHostToDevice);
    cudaMemcpy (bdevice, b, numBytes, cudaMemcpyHostToDevice);

    matMult_shared<<<blocks, threads>>>(adevice, bdevice, N, cdevice);
    cudaDeviceSynchronize();
    cudaMemcpy(c, cdevice, numBytes, cudaMemcpyDeviceToHost);

    //������������ ������ �� GPU � CPU
    cudaFree(adevice);
    cudaFree(bdevice);
    cudaFree(cdevice);

    free(a);
    free(b);
    free(c);

    return 0;
}