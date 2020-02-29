#include <iostream>
#include <fstream>
#include <string>
#include "function.hpp"
using namespace std;

/*Метод левых прямоуглоьников*/
//Сумма элементов
double sum_rectangle(double *Y, unsigned int n)
{
	double sum = 0;
	unsigned int i;

	for (i = 1; i < n; ++i) {
		sum += Y[i];
	}

	return sum;
}

/*Метод трапеций*/
//Сумма элементов
double sum_trapeze(double *Y, unsigned int n)
{
	double sum = 0;
	unsigned int i;

	for (i = 1; i < n - 1; ++i) {
		sum += Y[i];
	}

	return sum;
}

/*Метод парабол (Симпосна)*/
//Сумма чётных элементов
double even_sum(double *Y, unsigned int n)
{
	double sum = 0;
	unsigned int i;

	for (i = 2; i < n - 2; ++i) {
		if (!(i % 2)) {
			sum += Y[i];
		}
	}

	return sum;
}

//Сумма нечётных элементов
double odd_sum(double *Y, unsigned int n)
{
	double sum = 0;
	unsigned int i;

	for (i = 1; i < n - 1; ++i) {
		if ((i % 2)) {
			sum += Y[i];
		}
	}

	return sum;
}

//Основная функция
int main()
{
	double k, lim_a = 0, lim_b = 5, integral;
	unsigned int i, j, n = 11;
	//Расчёт h
	double h = (lim_b - lim_a) / (n - 1);

	setlocale(0, "");

	while (1) {
		k = 0;
		char inp_str[20];
		cout << "\nЧтобы посчитать интеграл методом парабол, введите \"1\"";
		cout << "\nЧтобы посчитать интеграл методом трапеций, введите \"2\"";
		cout << "\nЧтобы посчитать интеграл методом прямоугольников, введите \"3\"";
		cout << "\nЧтобы выйти из системы, введите \"quit\"";
		cout << "\nВвод >> ";
		cin >> inp_str;
		if (!strcmp(inp_str, "1")) {
			printf("h = %lf\n", h);
			//Выделение памяти для массивов
			double *X = new double[n];
			double *Y = new double[n];

			//Заполнение X
			for (i = 0; i < n; ++i) {
				X[i] = k;
				k += h;
			}
			//Заполнение Y
			for (i = 0; i < n; ++i) {
				Y[i] = function(X[i]);
			}

			//Вывод массивов на экран
			for (i = 0; i < n; ++i) {
				printf("\nX[%d] = %lf|Y[%d] = %lf", i + 1, X[i], i + 1, Y[i]);
			}

			//Численное интегрирование по Гомеру Симпсону
			integral = (h / 3) * (Y[0] + Y[n] + 4 * odd_sum(Y, n) + 2 * even_sum(Y, n));
			cout << "\n\nРешение интеграла: " << integral << endl;
			delete[] X;
			delete[] Y;
		} else if (!strcmp(inp_str, "2")) {
			printf("h = %lf\n", h);

			//Выделение памяти для массивов
			double *X = new double[n];
			double *Y = new double[n];

			//Заполнение X
			for (i = 0; i < n; ++i) {
				X[i] = k;
				k += h;
			}
			//Заполнение Y
			for (i = 0; i < n; ++i) {
				Y[i] = function(X[i]);
			}

			//Вывод массивов на экран
			for (i = 0; i < n; ++i) {
				printf("\nX[%d] = %lf|Y[%d] = %lf", i + 1, X[i], i + 1, Y[i]);
			}

			//Численное интегрирование методом трапеций
			integral = (h / 2) * (Y[0] + 2 * sum_trapeze(Y, n) + Y[n - 1]);
			cout << "\n\nРешение интеграла: " << integral << endl;
			delete[] X;
			delete[] Y;
		} else if (!strcmp(inp_str, "3")) {
			printf("h = %lf\n", h);

			//Выделение памяти для массивов
			double *X = new double[n];
			double *Y = new double[n];

			//Заполнение X
			for (i = 0; i < n; ++i) {
				X[i] = k;
				k += h;
			}
			//Заполнение Y
			for (i = 0; i < n; ++i) {
				Y[i] = function(X[i]);
			}

			//Вывод массивов на экран
			for (i = 0; i < n; ++i) {
				printf("\nX[%d] = %lf|Y[%d] = %lf", i + 1, X[i], i + 1, Y[i]);
			}

			//Численное интегрирование методом прямоугольников
			integral = h * sum_rectangle(Y, n);
			cout << "\n\nРешение интеграла: " << integral << endl;
			delete[] X;
			delete[] Y;
		} else if (!strcmp(inp_str, "quit") || !strcmp(inp_str, "Quit")) {
			cout << "\nВсего доброго. До новых встреч...\n";
			break;
		} else {
			cout << "\nВведены некорректные данные!";
		}
	}

	system("pause");
	return 0;
}