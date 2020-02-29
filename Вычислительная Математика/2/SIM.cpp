#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

void print_matr(double **M, unsigned int n, unsigned int m)
{
	unsigned int i, j;

	cout << endl;
	for (i = 0; i < n; ++i) {
		for (j = 0; j < m; ++j) {
			cout << M[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

double **multiple_ext(double **M, double multiplier, unsigned int n, unsigned int m)
{
	unsigned int i, j;
	double **A = new double *[n];

	for (i = 0; i < n; ++i) {
		A[i] = new double[m];
	}

	for (i = 0; i < n; ++i) {
		for (j = 0; j < m; ++j) {
			A[i][j] = M[i][j] * multiplier;
		}
	}

	return A;
}

double **multiple(double **C, double **X, unsigned int n, unsigned int m)
{
	unsigned int i, j, k;
	double **M = new double *[n];

	for (i = 0; i < n; ++i) {
		M[i] = new double[n];
	}

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			M[i][j] = 0;
			for (k = 0; k < n; ++k) {
				M[i][j] += C[i][k] * X[k][j];
			}
		}
	}

	return M;
}

double **sum(double **B, double **Temp1, unsigned int n, unsigned int m)
{
	unsigned int i, j;
	double **M = new double *[n];

	for (i = 0; i < n; ++i) {
		M[i] = new double[m];
	}

	for (i = 0; i < n; ++i) {
		for (j = 0; j < m; ++j) {
			M[i][j] = B[i][j] + Temp1[i][j];
		}
	}

	return M;
}

double **div_row(double **M, unsigned int n, unsigned int index, double divider)
{
	unsigned int i;

	for (i = 0; i < n; ++i) {
		M[index][i] /= divider;
	}

	return M;
}

double norm(double **M, unsigned int n, unsigned int m)
{
	unsigned int i, j;
	double norm = 0, sum = 0;

	for (i = 0; i < n; ++i) {
		for (j = 0; j < m; ++j) {
			sum += fabs(M[i][j]);
		}
		if (sum > norm) {
			norm = sum;
		}
		sum = 0;
	}

	return norm;
}

double **matrix(unsigned int n, unsigned int m)
{
	unsigned int i;

	double **M = new double *[n];
	for (i = 0; i < n; ++i) {
		M[i] = new double[m];
	}

	return M;
}

int main()
{
	setlocale(0, "");
	double eps;

	unsigned int i, j, n = 0;
	char *file = "matr4.txt", symb;
	setlocale(0, "");
	FILE *f = fopen(file, "r");

	cout << "Введите требуемую точность решения: ";
	cin >> eps;

	//Вычисляем размерность матрицы
	while (fscanf(f, "%c", &symb) != EOF) {
		switch (symb) {
		case '\n':
			++n;
			break;
		}
	}

	unsigned int m = n + 1;
	double **M = matrix(n, m);

	fclose(f);
	//Считываем матрицу
	ifstream f1(file);
	while (!f1.eof()) {
		for (i = 0; i < n; ++i) {
			for (j = 0; j < m; ++j) {
				f1 >> M[i][j];
			}
		}
	}

	f1.close();
	//Выделение памяти для матриц
	double **B = matrix(n, 1);
	double **X = matrix(n, 1);
	double **C = matrix(n, n);

	//Решение
	cout << "Расширенная матрица:" << endl;
	print_matr(M, n, m);

	//Приводим к виду удобному для итераций
	for (i = 0; i < n; i++) {
		M = div_row(M, m, i, M[i][i]);
	}

	cout << "Расширенная матрица (оптимизированная):" << endl;
	print_matr(M, n, m);

	//Заполняем матрицу C
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			C[i][j] = M[i][j];
		}
	}

	for (i = 0; i < n; i++) {
		C[i][i] = 0;
	}

	cout << "Основная матрица:" << endl;
	print_matr(C, n, n);

	//Заполняем матрицу B
	for (i = 0; i < n; ++i) {
		B[i][0] = M[i][n];
	}

	delete M;
	cout << "Вектор В:" << endl;
	print_matr(B, n, 1);

	//Расчет норм
	double B_norm = norm(B, n, 1);
	double C_norm = norm(C, n, n);
	cout << "Норма(C) = " << C_norm << endl << endl;
	cout << "Норма(B) = " << B_norm << endl << endl;

	if (C_norm >= 1) {
		cout << "Невозможно найти решение системы СЛАУ, так как Норма(С) ≥ 1" << endl << endl;
		system("pause");
		return -1;
	}

	//Расчет числа итераций
	unsigned int count;
	double result = log((eps * (1 - C_norm)) / B_norm) / log(C_norm) + 1;

	if (result == floor(result)) {
		count = (unsigned int)result;
	} else {
		count = (unsigned int)(floor(result) + 1);
	}

	cout << "Число итераций, требуемых для нахождения решения с заданной точностью: " << count << endl << endl;
	//system("pause");

	//Расчет иксов (итерации)
	for (i = 0; i < count; i++) {
		double **Temp = matrix(n, 1);
		Temp = multiple(C, X, n, 1);
		double **Temp1 = matrix(n, 1);
		Temp1 = multiple_ext(Temp, -1, n, 1);
		Temp = X;
		X = sum(B, Temp1, n, 1);
		delete Temp;
		delete Temp1;
	}

	cout << endl << "Вектор Х:" << endl;
	print_matr(X, n, 1);

	system("pause");
	return 0;
}