#include <string>
#include <iostream>
#include "cryptography.hpp"
using namespace std;

int main()
{
	srand(time(NULL));

	string Input, Filename;
	int check;
	double time;

	do {
		cout << "Введите имя файла\nВвод >> ";
		cin >> Filename;
	} while(check_file(Filename));

	do {
		cout << WHITE << "Для начала работы, введите:\n" << NORMAL;
		cout << GREEN << "1 - Шамир\n" << NORMAL;
		cout << BLUE << "2 - Эль-Гамаль\n" << NORMAL;
		cout << YELLOW << "3 - Вернам\n" << NORMAL;
		cout << PURPLE << "4 - RSA\n" << NORMAL;
		cout << CYAN << "5 - Ввести имя нового файла (текущее имя: " << Filename << ")\n" << NORMAL;
		cout << WHITE << "T - Удалить зашифрованные/дефишрованные файлы\n" << NORMAL;
		cout <<RED << "Q - Выход из программы\n" << NORMAL;
		cout << "Ввод >> ";
		cin >> Input;

		switch(Input[0]) {
			case '1':
				cout << "\nШифрование (алгоритм Шамира)\n";
				time = clock();

				shamir_encrypt(Filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
				cout << "Дешифрование (алгоритм Шамира)\n";
				time = clock();

				shamir_decrypt(Filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
			break;
			case '2':
				cout << "\nШифрование (алгоритм Эль-Гамаля)\n";
				time = clock();

				el_gamal_encrypt(Filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
				cout << "Дешифрование (алгоритм Эль-Гамаля)\n";
				time = clock();

				el_gamal_decrypt(Filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
			break;
			case '3':
				cout << "\nШифрование (алгоритм Вернама)\n";
				time = clock();

				vernam_encrypt(Filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
				cout << "Дешифрование (алгоритм Вернама)\n";
				time = clock();

				vernam_decrypt(Filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
			break;
			case '4':
				cout << "\nШифрование (алгоритм RSA)\n";
				time = clock();

				rsa_encrypt(Filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
				cout << "Дешифрование (алгоритм RSA)\n";
				time = clock();

				rsa_decrypt(Filename);

				time = clock() - time;
				cout << WHITE << "Время выполнения: " << time / CLOCKS_PER_SEC << " сек\n" << NORMAL;
			break;
			case '5':
				cout << "Введите имя файла\nВвод >> ";
				cin >> Input;
				if(!check_file(Input)) {
					Filename = Input;
				}
			break;
			case 't':
			case 'T':
				check = system("rm res/*.dec");
				check = system("rm res/*.enc");
			break;
			case 'q':
			case 'Q':
			break;
			default:
				cout << RED << "Ошибка! Введены неверные данные.\n" << NORMAL;
			break;
		};
		cout << endl;
	} while(Input[0] != 'q' && Input[0] != 'Q');

	return 0;
}

