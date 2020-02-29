#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

//Числитель
double numerator(double *x, double xx, unsigned int n, unsigned int index)
{
	unsigned int i, j;
	double multiple = 1;

	for (i = 0; i < n; ++i) {
		if (i != index) {
			multiple *= xx - x[i];
		}
	}

	return multiple;
}

//Знаменатель
double denominator(double *x, unsigned int n, unsigned int index)
{
	unsigned int i;
	double multiple = 1;

	for (i = 0; i < n; ++i) {
		if (i != index) {
			multiple *= x[index] - x[i];
		}
	}

	return multiple;
}

int main()
{
	unsigned int i = 0, n = 1;
	char *file = "file.txt", symb;
	char *buffer = new char[100];
	setlocale(0, "");
	ifstream f(file);

	f.getline(buffer, 100, '\n');
	f.close();
	
	//Вычисляем размер массивов
	while (buffer[i] != '\0') {
		if (buffer[i] == ' ') {
			++n;
		}
		++i;
	}

	delete[] buffer;
	double *X = new double[n];
	double *Y = new double[n];

	//Считываем матрицу
	ifstream f1(file);
	while (!f1.eof()) {
		for (i = 0; i < n; ++i) {
			f1 >> X[i];
		}
		for (i = 0; i < n; ++i) {
			f1 >> Y[i];
		}
	}
	f1.close();
	for (i = 0; i < n; ++i) {
		printf("X[%d] = %lf, Y[%d] = %lf\n", i + 1, X[i], i + i, Y[i]);
	}

	double P = 0;
	//Константа Х
	double xx;
	xx = ((-5 - sqrt(89)) / 4);
	/*cout << "\nВведите значение константы X: ";
	cin >> xx;*/

	//Вычисление полинома
	for (i = 0; i < n; ++i) {
		P += Y[i] * (numerator(X, xx, n, i) / denominator(X, n, i));
	}

	printf("\nP = %.10lf\n", P);
	system("pause");
	return 0;
}