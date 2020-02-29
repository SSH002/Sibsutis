#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "function.hpp"
using namespace std;

int main()
{
	//Начальные условия
	double x0 = 0, y0 = 1, h = 0.1;
	unsigned int i;
	int d1, d2;
	char inp_str[20];

	setlocale(0, "");

	while (1) {
		cout << "\nЧтобы посчитать диффер. уравнение методом Эйлера, введите \"1\"";
		cout << "\nЧтобы посчитать диффер. уравнение модифиц. методом Эйлера, введите \"2\"";
		cout << "\nЧтобы выйти из системы, введите \"quit\"";
		cout << "\nВвод >> ";
		cin >> inp_str;
		if (!strcmp(inp_str, "1")) {
			cout << "\nВведите нижнюю границу функции: ";
			cout << "\nВвод >> ";
			cin >> d1;
			cout << "\nВведите верхнюю границу функции: ";
			cout << "\nВвод >> ";
			cin >> d2;
			unsigned int n = (abs(d2 - d1) / h) + 1;
			double *X = new double[n];
			double *Y = new double[n];
			double *Func = new double[n];
			X[0] = x0;
			Y[0] = y0;
			for (i = 0; i < n; ++i) {
				Func[i] = function(X[i], Y[i]);
				X[i + 1] = X[i] + h;
				Y[i + 1] = Y[i] + h * Func[i];
			}
			for (i = 0; i < n; ++i) {
				cout << "\nX[" << i << "] = " << X[i] << ", Y[" << i << "] = " << Y[i] << ", F[" << i << "] = " << Func[i];
			}
			cout << endl;
		} else if (!strcmp(inp_str, "2")) {
			cout << "\nВведите нижнюю границу функции: ";
			cout << "\nВвод >> ";
			cin >> d1;
			cout << "\nВведите верхнюю границу функции: ";
			cout << "\nВвод >> ";
			cin >> d2;
			unsigned int n = (abs(d2 - d1) / h) + 1;
			double *X = new double[n];
			double *Y = new double[n];
			double *dY = new double[n];
			double *Func = new double[n];
			X[0] = x0;
			Y[0] = y0;
			for (i = 0; i < n; ++i) {
				Func[i] = function(X[i], Y[i]);
				X[i + 1] = X[i] + h;
				dY[i] = h * function(X[i] + h / 2, Y[i] + h * Func[i] / 2);
				Y[i + 1] = Y[i] + dY[i];
			}
			for (i = 0; i < n; ++i) {
				cout << "\nX[" << i << "] = " << X[i] << ", Y[" << i << "] = " << Y[i] << ", dY[" << i << "] = " << dY[i] << ", F[" << i << "] = " << Func[i];
			}
			cout << endl;
		} else if (!strcmp(inp_str, "quit") || !strcmp(inp_str, "Quit")|| !strcmp(inp_str, "QUIT")) {
			break;
		} else {
			cout << "\nОшибка! Введены некорректные данные.";
		}
	}
		return 0;
	}