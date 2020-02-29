#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
using namespace std;

int max(double **m, int index, int size)
{
	int j, d;
	int max = -2100000000;
	for (j = 0; j < size; ++j) {
		if (m[j][index] > max) {
			max = m[j][index];
			d = j;
		}	
	}
	
	return d;
}

int main()
{
	int i, j, n = 0;
	char *file = "matr4", symb;
	setlocale(0, "");
	FILE *f = fopen(file, "r");

	//Вычисляем размерность матрицы
	while (fscanf(f, "%c", &symb) != EOF) {
		if (sumb == '\n') {
				++n;
		}
	}

	int m = n + 1;
	double **M = new double *[n];
	for (i = 0; i < n; ++i) {
		M[i] = new double [m];
	}

	//Считываем матрицу
	ifstream f1(file);
	while (!f1.eof()) {
		for (i = 0; i < n; ++i) {
			for (j = 0; j < m; ++j) {
				f1 >> M[i][j];
			}
		}
	}

	cout << "Матрица: \n";
	for (i = 0; i < n; ++i) {
		for (j = 0; j < m; ++j) {
			cout << M[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	//Модифицированный метод Гаусса
	double  tmp, *X = new double[n];
	double *B = new double[m];
	int k, d;

	//Выбор ведущего элемента и перестановка строк
	/*for (i = 0; i < n; ++i) {
		d = max(M, i, n);
		for (k = 0; k < m; k++) {
			B[k] = M[i][k];
			M[i][k] = M[d][k];
			M[d][k] = B[k];
		}
	}*/

	delete[] B;

	//Прямой ход, приведение к верхнетреугольному виду
	cout << "Обновлённая матрица: \n";
	for (i = 0; i < n; ++i) {
		for (j = 0; j < m; ++j) {
			cout << M[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	for (i = 0; i < n; ++i) {
		tmp = M[i][i];
		for (j = n; j >= i; --j) {
			M[i][j] /= tmp;
		}
		for (j = i + 1; j < n; ++j) {
			tmp = M[j][i];
			for (k = n; k >= i; --k) {
				M[j][k] -= tmp * M[i][k];
			}
		}
	}

	//Обратный ход
	X[n - 1] = M[n - 1][n];
	for (i = n - 2; i >= 0; --i) {
		X[i] = M[i][n];
		for (j = i + 1; j < n; ++j) {
			X[i] -= M[i][j] * X[j];
		}
	}

	//Выводим решения
	for (i = 0; i < n; ++i) {
		cout << X[i] << " ";
	}
	cout << endl;

	delete[] M;
	delete[] X;

	system("PAUSE");
	return 0;
}