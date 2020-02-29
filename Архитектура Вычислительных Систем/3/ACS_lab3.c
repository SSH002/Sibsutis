#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define hertz 2300000000			//Дома
//#define hertz 2500000000			//В классе
/*Число Пи находится с помощью ряда Н*/

//Возвращает порядок числа "number"
unsigned int number_order(unsigned int number, unsigned int order)
{
	if ((number /= 10) == 0) {
		return order;
	}

	return number_order(number, ++order);
}

//Функция для нахождения числа Пи(замеры с помощью clock)
double pi_clock(int N)
{
	double pi = 3.0, a = 2.0, b = pi, c = 4.0;
	int i, j = 1;
	double start, end;

	start = clock();

	for (i = 0; i < N; ++i, a += 2, b += 2, c += 2, j *= -1) {
		pi += j * (4 / (a * b * c));
	}

	end = clock();

	return (end - start) / CLOCKS_PER_SEC;
}

//Функция для нахождения числа Пи(замеры с помощью gettimeofday)
double pi_gettimeofday(int N)
{
	double pi = 3.0, a = 2.0, b = pi, c = 4.0;
	int i, j = 1;
	double sec, usec;
	struct timeval tv1;
        struct timeval tv2;

	gettimeofday(&tv1, NULL);

	for (i = 0; i < N; ++i, a += 2, b += 2, c += 2, j *= -1) {
		pi += j * (4 / (a * b * c));
	}

	gettimeofday(&tv2, NULL);

	sec = (double)fabs(tv2.tv_sec - tv1.tv_sec);
	usec = (double)fabs(tv2.tv_usec - tv1.tv_usec);

	return sec + usec / pow(10, number_order(usec, 1));
}

//Функция для нахождения числа Пи(замеры с помощью tsc)
double pi_tsc(int N)
{
	double pi = 3.0, a = 2.0, b = pi, c = 4.0;
	int i, j = 1;

	uint32_t high, low;
	__asm__ __volatile__ (
	"rdtsc\n"
	"movl %%edx, %0\n"
	"movl %%eax, %1\n"
		: "=r" (high), "=r" (low)
		:: "%rax", "%rbx", "%rcx", "%rdx"
	);
	uint64_t ticks = ((uint64_t)high << 32) | low;

	for (i = 0; i < N; ++i, a += 2, b += 2, c += 2, j *= -1) {
		pi += j * (4 / (a * b * c));
	}

	__asm__ __volatile__ (
		"rdtsc\n"
		"movl %%edx, %0\n"
		"movl %%eax, %1\n"
		: "=r" (high), "=r" (low)
		:: "%rax", "%rbx", "%rcx", "%rdx"
	);
	ticks = (((uint64_t)high << 32) | low) - ticks;

	return (double)ticks / (double)hertz;
}

int main(int argc, char *argv[])
{
	int N;

	if (argc != 2) {
		printf("Ошибка! Некорректные аргументы. Пример запуска: \"./lb3 \'1\'-\'9\'\"\n");
		return 1;
	} else {
		switch(argv[1][0]) {
			case '1':
				N = 10;
			break;
			case '2':
				N = 100;
			break;
			case '3':
				N = 1000;
			break;
			case '4':
				N = 10000;
			break;
			case '5':
				N = 100000;
			break;
			case '6':
				N = 1000000;
			break;
			case '7':
				N = 10000000;
			break;
			case '8':
				N = 100000000;
			break;
			case '9':
				N = 1000000000;
			break;
			default:
				printf("Ошибка! Введён некорректный символ. Пример запуска: \"./lb3 \'2\'-\'9\'\"\n");
				return 2;
			break;
		};
	}

	struct timeval tv1;
	struct timeval tv2;

	printf("\n      clock   gettimeofday   tsc\n");
	for (int i = 0; i < 10; ++i) {
		printf("[%d]|%lf | %lf | %lf|\n", i + 1, pi_clock(N), pi_gettimeofday(N), pi_tsc(N));
	}

	return 0;
}