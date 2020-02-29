#include <iostream>
#include <fstream>
#include <string>
#include "function.hpp"
using namespace std;

/*����� ����� ���������������*/
//����� ���������
double sum_rectangle(double *Y, unsigned int n)
{
	double sum = 0;
	unsigned int i;

	for (i = 1; i < n; ++i) {
		sum += Y[i];
	}

	return sum;
}

/*����� ��������*/
//����� ���������
double sum_trapeze(double *Y, unsigned int n)
{
	double sum = 0;
	unsigned int i;

	for (i = 1; i < n - 1; ++i) {
		sum += Y[i];
	}

	return sum;
}

/*����� ������� (��������)*/
//����� ������ ���������
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

//����� �������� ���������
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

//�������� �������
int main()
{
	double k, lim_a = 0, lim_b = 5, integral;
	unsigned int i, j, n = 11;
	//������ h
	double h = (lim_b - lim_a) / (n - 1);

	setlocale(0, "");

	while (1) {
		k = 0;
		char inp_str[20];
		cout << "\n����� ��������� �������� ������� �������, ������� \"1\"";
		cout << "\n����� ��������� �������� ������� ��������, ������� \"2\"";
		cout << "\n����� ��������� �������� ������� ���������������, ������� \"3\"";
		cout << "\n����� ����� �� �������, ������� \"quit\"";
		cout << "\n���� >> ";
		cin >> inp_str;
		if (!strcmp(inp_str, "1")) {
			printf("h = %lf\n", h);
			//��������� ������ ��� ��������
			double *X = new double[n];
			double *Y = new double[n];

			//���������� X
			for (i = 0; i < n; ++i) {
				X[i] = k;
				k += h;
			}
			//���������� Y
			for (i = 0; i < n; ++i) {
				Y[i] = function(X[i]);
			}

			//����� �������� �� �����
			for (i = 0; i < n; ++i) {
				printf("\nX[%d] = %lf|Y[%d] = %lf", i + 1, X[i], i + 1, Y[i]);
			}

			//��������� �������������� �� ������ ��������
			integral = (h / 3) * (Y[0] + Y[n] + 4 * odd_sum(Y, n) + 2 * even_sum(Y, n));
			cout << "\n\n������� ���������: " << integral << endl;
			delete[] X;
			delete[] Y;
		} else if (!strcmp(inp_str, "2")) {
			printf("h = %lf\n", h);

			//��������� ������ ��� ��������
			double *X = new double[n];
			double *Y = new double[n];

			//���������� X
			for (i = 0; i < n; ++i) {
				X[i] = k;
				k += h;
			}
			//���������� Y
			for (i = 0; i < n; ++i) {
				Y[i] = function(X[i]);
			}

			//����� �������� �� �����
			for (i = 0; i < n; ++i) {
				printf("\nX[%d] = %lf|Y[%d] = %lf", i + 1, X[i], i + 1, Y[i]);
			}

			//��������� �������������� ������� ��������
			integral = (h / 2) * (Y[0] + 2 * sum_trapeze(Y, n) + Y[n - 1]);
			cout << "\n\n������� ���������: " << integral << endl;
			delete[] X;
			delete[] Y;
		} else if (!strcmp(inp_str, "3")) {
			printf("h = %lf\n", h);

			//��������� ������ ��� ��������
			double *X = new double[n];
			double *Y = new double[n];

			//���������� X
			for (i = 0; i < n; ++i) {
				X[i] = k;
				k += h;
			}
			//���������� Y
			for (i = 0; i < n; ++i) {
				Y[i] = function(X[i]);
			}

			//����� �������� �� �����
			for (i = 0; i < n; ++i) {
				printf("\nX[%d] = %lf|Y[%d] = %lf", i + 1, X[i], i + 1, Y[i]);
			}

			//��������� �������������� ������� ���������������
			integral = h * sum_rectangle(Y, n);
			cout << "\n\n������� ���������: " << integral << endl;
			delete[] X;
			delete[] Y;
		} else if (!strcmp(inp_str, "quit") || !strcmp(inp_str, "Quit")) {
			cout << "\n����� �������. �� ����� ������...\n";
			break;
		} else {
			cout << "\n������� ������������ ������!";
		}
	}

	system("pause");
	return 0;
}