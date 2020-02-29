#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

unsigned int factorial(unsigned int index)
{
	if (index == 0 || index == 1) {
		return 1;
	}

	return index * factorial(index - 1);
}

double multiple_q(double xx, double *X, unsigned int n)
{
	unsigned int i;
	double h = X[1] - X[0];

	double multiple = (xx - X[0]) / h;
	for (i = 1; i < n; i++) {
		multiple *= ((xx - X[0]) / h) - i;
	}

	return multiple;
}

int main()
{
	setlocale(0, "");
	double xx = 1.44;
	unsigned int i, j, n = 0;

	char *file = "file.txt";
	char *buffer = new char[100];
	setlocale(0, "");
	ifstream f(file);

	f.getline(buffer, 100, '\n');
	f.close();

	//Вычисляем размер массивов
	for (i = 0; i < 100 && buffer[i] != '\0'; ++i) {
		if (buffer[i] == ' ') {
			++n;
		}
	}

	delete[] buffer;
	double *X = new double[n];
	double **Y = new double *[n];
	for (j = 0; j < n; ++j) {
		Y[j] = new double[n - j];
	}

	//Считываем матрицу
	ifstream f1(file);
	while (!f1.eof()) {
		for (i = 0; i < n; ++i) {
			f1 >> X[i];
		}
		for (j = 0; j < n; ++j) {
			f1 >> Y[0][j];
		}
	}
	f1.close();

	//Вычисление dY
	for (i = 1; i < n; ++i) {
		for (j = 0; j < n - i; ++j) {
			Y[i][j] = Y[i - 1][j + 1] - Y[i - 1][j];
		}
	}

	for (i = 0; i < n; ++i) {
		cout << "X[" << i << "] = " << X[i] << ", Y[0][" << i << "] = " << Y[0][i] << endl;
	}
	for (i = 1; i < n; ++i) {
		for (j = 0; j < n - i; ++j) {
			cout << "dY" << i << " = " << Y[i][j] << endl;
		}
	}

	double *dY = new double[n];
	for (i = 0; i < n; ++i) {
		dY[i] = Y[i][0];
	}
	delete[] Y;

	double P = dY[0];
	//Метод Ньютона
	for (i = 1; i < n; ++i) {
		P += (dY[i] / factorial(i)) * multiple_q(xx, X, i);
	}
	cout << "\nP = " << P << endl;

	delete[] X;
	delete[] dY;

	system("pause");
	return 0;
}