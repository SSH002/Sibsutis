#include <stdio.h>
#include <cuda.h>
#include <math.h>
#include <stdlib.h>

#define BLOCK_SIZE 32

//Вывод матрицы
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
    int i = threadIdx.x + blockIdx.x * blockDim.x; //Один поток -> один элемент матрицы
    int j = threadIdx.y + blockIdx.y * blockDim.y;

    float sum = 0.0f;
    for (int k = 0; k < n; ++k)
        sum += a[i * n + k] * b[k * n + j];
    c[i * n + j] = sum;

}

__global__ void matMult_shared(float *a, float *b, int n, float* c)
{
    int aBegin = n * BLOCK_SIZE * blockIdx.y; //Индекс начала первой подматрицы А, которую обрабатывает блок
    int aEnd = aBegin + n - 1; //Индекс конца подматрицы А, которую обрабатывает блок
    int aStep = BLOCK_SIZE; //Шаг для перебора матриц А

    int bBegin = BLOCK_SIZE * blockIdx.x; //Индекс начала первой подматрицы В, которую обрабатывает блок
    int bStep = BLOCK_SIZE * n; //Шаг для перебора подматиц B

    float sum = 0.0f; //Вычисление элемента подматрицы

    for (int sub_A = aBegin, sub_B = bBegin; sub_A <= aEnd; sub_A += aStep, sub_B += bStep) {
        //Выделение разделямой памяти для подматриц
        __shared__ float buffer_A[BLOCK_SIZE][BLOCK_SIZE];
        __shared__ float buffer_B[BLOCK_SIZE][BLOCK_SIZE];

        //Загрузка подматриц А и В из глобальной памяти в разделяемую
		buffer_A[threadIdx.y][threadIdx.x] = a[sub_A + n * threadIdx.y + threadIdx.x];
        buffer_B[threadIdx.y][threadIdx.x] = b[sub_B + n * threadIdx.y + threadIdx.x];

        __syncthreads(); // Убедимся, что подматрицы полностью загружены

        //Перемножение двух матриц
        for (int k = 0; k < BLOCK_SIZE; k++)
            sum += buffer_A[threadIdx.x][k] * buffer_B[k][threadIdx.x];
        __syncthreads(); // Убедимся, что подматрицы никому больше не нужны
    }

    int cBegin = n * BLOCK_SIZE * blockIdx.x + BLOCK_SIZE * blockIdx.x; //Индекс результирующего элемента в глобальной памяти
    c[cBegin + n * threadIdx.y + threadIdx.x] = sum; //Запись элемента в глобальную память
}

int main (int argc, char *  argv [])
{
    //Ввод размерности матрицы
    if(argc < 2){
        fprintf(stderr, "Введите размер матрицы: ");
        return -1;
    }

    //Проверка входных данных
    int N = atoi(argv[1]);
	if(N % BLOCK_SIZE) {
        fprintf(stderr, "Измените размер\n");
        return -1;
    }

    int numBytes = N * N * sizeof(float);

    //Инициализация исходного массива
    //Выделение памяти на host
    float *a = (float*)malloc(numBytes); //Матрица А
    float *b = (float*)malloc(numBytes); //Матрица B
    float *c = (float*)malloc(numBytes); //Матрица С

    //Заполнение матриц
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            int k = N * i + j;
            a[k] = 2.0f;
            b[k] = 1.0f;
        }

    float *adevice, *bdevice, *cdevice;

    //Выделение памяти на GPU
    cudaMalloc((void**)&adevice, numBytes);
	cudaMalloc((void**)&bdevice, numBytes);
    cudaMalloc((void**)&cdevice, numBytes);


    //Задание сетки нитей и блоков
    dim3 threads(BLOCK_SIZE, BLOCK_SIZE);
    dim3 blocks(N / threads.x, N / threads.y);

    //Копирование матриц А и B c host на device
    //Инициализация происходит на CPU -> Копируем на GPU
    cudaMemcpy(adevice, a, numBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(bdevice, b, numBytes, cudaMemcpyHostToDevice);

    //Запуск функции
    matMult_global<<<blocks, threads>>>(adevice, bdevice, N, cdevice);
    cudaDeviceSynchronize();

    //Копирование, вычисленной матрицы С с device на host
    //Возвращаем обратно -> Результат
    cudaMemcpy(c, cdevice, numBytes, cudaMemcpyDeviceToHost);

	cudaMemcpy (adevice, a, numBytes, cudaMemcpyHostToDevice);
    cudaMemcpy (bdevice, b, numBytes, cudaMemcpyHostToDevice);

    matMult_shared<<<blocks, threads>>>(adevice, bdevice, N, cdevice);
    cudaDeviceSynchronize();
    cudaMemcpy(c, cdevice, numBytes, cudaMemcpyDeviceToHost);

    //Освобождение памяти на GPU и CPU
    cudaFree(adevice);
    cudaFree(bdevice);
    cudaFree(cdevice);

    free(a);
    free(b);
    free(c);

    return 0;
}