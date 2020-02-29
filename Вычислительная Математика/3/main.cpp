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
		cout << "�������� ����� �������: \n";
		cout << "������� \"1\", ����� ������ ������� ������� ��������.\n";
		cout << "������� \"2\", ����� ������ ������� ������� ����.\n";
		cout << "������� \"3\", ����� ������ ������� ������� �������.\n";
		cout << "������� \"q\", ����� ����� �� �������.\n";
		cout << "����: ";
		cin >> choice;
		//����� �������� �� ���������, ����� ������ ��������� ��������� ���������
		x = 10;
		c = 0;
		buf = 0;
		lim_a = limit_a;
		lim_b = limit_b;
		//����� �������� �� ���������, ����� ������ ��������� ��������� ���������
		switch (choice) {
			//����� ��������
			case '1':
				cout << "������� ��������� �������� �������: ";
				cin >> eps;
				if ((func(lim_a)) * (func(lim_b)) < 0) {
					cout << "� ������� ��������� ����� ��������� ����� ��������\n";
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
					cout << "������� �������: " << c << endl;
				} else {
					cout << "������! � ������� ��������� ���������� ����� ��������\n";
					break;
				}
			break;
			/*����� ����*/
			case '2':
				cout << "������� ��������� �������� �������: ";
				cin >> eps;
				if ((func(lim_a)) * (func(lim_b)) < 0) {
					cout << "� ������� ��������� ����� ��������� ����� ����\n";
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
					cout << "������� �������: " << x << endl;
				} else {
					cout << "������! � ������� ��������� ���������� ����� ����\n";
					break;
				}
			break;
			/*����� ������*/
			case '3':						
				cout << "������� ��������� �������� �������: ";
				cin >> eps;
				if (((func(lim_a)) * (func(lim_b)) < 0) && (func1(lim_b)) && (func2(lim_b))) {
					cout << "� ������� ��������� ����� ��������� ����� �������\n";
					c = lim_b;
					while (abs(x - buf) > eps) {
						buf = c;
						x = c - (func(c) / func1(c));
						c = x;
					}
					cout << "������� �������: " << x << endl;
				} else {
					cout << "������! � ������� ��������� ���������� ����� �������\n";
					break;
				}
			break;
			/*����� �� ���������*/
			case 'q':
			break;
			/*������������ ������*/
			default:
				cout << "������! ������� �������� ������.\n";
			break;
		}
	}

	return 0;
}