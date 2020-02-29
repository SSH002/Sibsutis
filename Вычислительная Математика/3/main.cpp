#include <iostream>
#include "function.hpp"
using namespace std;

double find_c(double a, double b)
{
	return ((a + b) / 2);
}

int main()
{
	double x, eps, c, buf;
	double lim_a, lim_b;
	char choice = ' ';
	setlocale(0, "");

	while (choice != 'q') {
		cout << "Выберите метод решения: \n";
		cout << "Нажмите \"1\", чтобы решить систему методом бисекции.\n";
		cout << "Нажмите \"2\", чтобы решить систему методом хорд.\n";
		cout << "Нажмите \"3\", чтобы решить систему методом Ньютона.\n";
		cout << "Нажмите \"q\", чтобы выйти из системы.\n";
		cout << "Ввод: ";
		cin >> choice;
		//Задаём значения по умолчанию, чтобы корень уравнения находился корректно
		x = 10;
		c = 0;
		buf = 0;
		lim_a = limit_a;
		lim_b = limit_b;
		//Задаём значения по умолчанию, чтобы корень уравнения находился корректно
		switch (choice) {
			//Метод бисекции
			case '1':
				cout << "Введите требуемую точность решения: ";
				cin >> eps;
				if ((func(lim_a)) * (func(lim_b)) < 0) {
					cout << "К данному уравнению можно применить метод бисекции\n";
					while (x > eps) {
						c = find_c(lim_a, lim_b);
						if ((func(lim_a)) * (func(c)) < 0) {
							lim_b = c;
						} else if ((func(c)) * (func(lim_b)) < 0) {
							lim_a = c;
						}
						x = abs(c - buf);
						buf = c;
					}
					cout << "Решение системы: " << c << endl;
				} else {
					cout << "Ошибка! К данному уравнению неприменим метод бисекции\n";
					break;
				}
			break;
			/*Метод хорд*/
			case '2':
				cout << "Введите требуемую точность решения: ";
				cin >> eps;
				if ((func(lim_a)) * (func(lim_b)) < 0) {
					cout << "К данному уравнению можно применить метод хорд\n";
					if (func(lim_a) * func2(lim_a) > 0) {
						c = lim_a;
					} else if (func(lim_b) * func2(lim_b) > 0) {
						c = lim_b;
					}
					while (abs(x - buf) > eps) {
						buf = c;
						if ((func(lim_a)) * (func(c)) < 0) {
							x = lim_a - ((func(lim_a) * (c - lim_a)) / (func(c) - func(lim_a)));
						} else if ((func(c)) * (func(lim_b)) < 0) {
							x = c - ((func(c) * (lim_b - c)) / (func(lim_b) - func(c)));
						}
						c = x;
					}
					cout << "Решение системы: " << x << endl;
				} else {
					cout << "Ошибка! К данному уравнению неприменим метод хорд\n";
					break;
				}
			break;
			/*Метод Ньютон*/
			case '3':						
				cout << "Введите требуемую точность решения: ";
				cin >> eps;
				if (((func(lim_a)) * (func(lim_b)) < 0) && (func1(lim_b)) && (func2(lim_b))) {
					cout << "К данному уравнению можно применить метод Ньютона\n";
					c = lim_b;
					while (abs(x - buf) > eps) {
						buf = c;
						x = c - (func(c) / func1(c));
						c = x;
					}
					cout << "Решение системы: " << x << endl;
				} else {
					cout << "Ошибка! К данному уравнению неприменим метод Ньютона\n";
					break;
				}
			break;
			/*Выход из программы*/
			case 'q':
			break;
			/*Некорректные данные*/
			default:
				cout << "Ошибка! Введены неверные данные.\n";
			break;
		}
	}

	return 0;
}