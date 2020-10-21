#include <string>
#include <iostream>
#include "cryptography.hpp"
using namespace std;

int main()
{
	srand(time(NULL));

	string input, filename;
	double time;

	do {
		cout << "Введите имя файла\nВвод >> ";
		cin >> filename;
	} while(check_file(filename));

	do {
		cout << GREEN << "1 - Шамир\n" << NORMAL;
		cout << YELLOW << "2 - Эль-Гамаль\n" << NORMAL;
		cout << BLUE << "3 - Вернам\n" << NORMAL;
		cout << PURPLE << "4 - RSA\n" << NORMAL;
		cout << CYAN << "5 - Ввести имя нового файла (текущее имя: " << filename << ")\n" << NORMAL;
		cout << WHITE << "Q - Выход из программы\n" << NORMAL;
		cout << "Ввод >> ";
		cin >> input;

		switch(input[0]) {
			case '1':
				cout << "\nНачало шифрование (алгоритм Шамира)\n";
				time = clock();

				shamir_encrypt(filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
				cout << "Начало дешифрование (алгоритм Шамира)\n";
				time = clock();

				shamir_decrypt(filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
			break;
			case '2':
				cout << "\nНачало шифрования (алгоритм Эль-Гамаля)\n";
				time = clock();

				el_gamal_encrypt(filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
				cout << "Начало дешифрование (алгоритм Эль-Гамаля)\n";
				time = clock();

				el_gamal_decrypt(filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
			break;
			case '3':
				cout << "\nНачало шифрования (алгоритм Вернама)\n";
				time = clock();

				vernam_encrypt(filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
				cout << "Начало дешифрование (алгоритм Вернама)\n";
				time = clock();

				vernam_decrypt(filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
			break;
			case '4':
				cout << "\nНачало шифрования (алгоритм RSA)\n";
				time = clock();

				rsa_encrypt(filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
				cout << "Начало дешифрование (алгоритм RSA)\n";
				time = clock();

				rsa_decrypt(filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
			break;
			case '5':
				cout << "Введите имя файла\nВвод >> ";
				cin >> input;
				if(!check_file(input)) {
					filename = input;
				}
			break;
			case 'q':
			case 'Q':
			break;
			default:
				cout << RED << "Ошибка! Введены неверные данные.\n" << NORMAL;
			break;
		};
		cout << endl;
	} while(input[0] != 'q' && input[0] != 'Q');

	return 0;
}

