#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 16
//#define hertz 2500000000		//В классе
#define hertz 2300000000		//Дома
int *X, *Y, *vector;

struct Data {
	int lb, ub, size, number, cnst;
} data[NUM_THREADS];

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

//Выполняемая потоками функция
void *saxpy_blas(void *arg)
{
	uint64_t timer;

	struct Data *data2 = (struct Data *) arg;
	unsigned int i, j;

	vector = (int*) malloc(sizeof(int) * (*data2).size);
	X = (int*) malloc(sizeof(int) * (*data2).size);
	Y = (int*) malloc(sizeof(int) * (*data2).size);

	srand(time(NULL));
	for(i = (*data2).lb; i < (*data2).ub; ++i) {
		vector[i] = 0;
		X[i] = rand() % 100;
		Y[i] = rand() % 100;
	}

	timer = tsc_wtime();

	//Перемножение векторов
	for(i = (*data2).lb; i < (*data2).ub; ++i) {
		for(j = 0; j < (*data2).size; ++j) {
			vector[i] += (*data2).cnst * X[i] + Y[j];
		}
	}

	/*for(i = (*data2).lb; i < (*data2).ub; ++i) {
		printf("vector[%d] = %lf, поток[%d]\n", i, vector[i], (*data2).number);
	}
	printf("\n");*/

	timer = tsc_wtime() - timer;
	printf("Затрачено времни: %lf сек, Поток №%d\n", (double)timer / (double)hertz, (*data2).number);

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	if(argc != 3) {
		printf("Ошибка! Введены некорректные параметры.\n");
		printf("Пример ввода: ./lb5 \"Size\" \"Const\"\n");
		printf(", где Size - Кол-во элементов, Const - константа\n");
		return 1;
	}

	int i, check, n, n0 = atoi(argv[1]);

	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;

	//Инициализация потоков и задаём значения атрибутов
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	//Создаём нулевой поток...
	n = (int)((float)n0 / (float)NUM_THREADS);
	data[0].cnst = atoi(argv[2]);
	data[0].size = n0;
	data[0].lb = 0;
	data[0].ub = n + (n0 % NUM_THREADS);
	data[0].number = 0;
	check = pthread_create(&thread[0], &attr, saxpy_blas, (void*)&data[0]);
	if(check != 0) {
		printf("Создание нулевого потока окончилось неудачей!\n");
		return 2;
	}

	//...и остальные
	for(i = 1; i < NUM_THREADS; ++i) {
		data[i].cnst = atoi(argv[2]);
		data[i].size = n0;
		data[i].lb = data[i - 1].ub;
		data[i].ub = data[i].lb + n;
		data[i].number = i;

		check = pthread_create(&thread[i], &attr, saxpy_blas, (void*)&data[i]);
		if(check != 0)	{
			printf("Создание потока №%d окончилось неудачей!\n", i);
			return 2;
		}
	}

	//Проверка результата работы потоков
	for(i = 0; i < NUM_THREADS; ++i) {
		check = pthread_join(thread[i], NULL);
		if(check) {
			printf("Объединение потока №%d окончилось неудачей!\n", i);
			return 3;
		}
	}
	printf("\n");

	return 0;
}