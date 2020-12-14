#include <string>
#include <iostream>
#include <filesystem>
#include "cryptography.hpp"
using namespace std::filesystem;

int main()
{
	srand(time(0));

	string file_name = "res/12.bmp", input;
	lint a, d, g, n, p, q, r, y;
	uint64 size;
	bool check;
	int out;

	do {
		cout << WHITE << "Для начала работы, введите:\n" << NORMAL;
		cout << GREEN << "1 - RSA\n" << NORMAL;
		cout << BLUE << "2 - Эль-Гамаль\n" << NORMAL;
		cout << YELLOW << "3 - ГОСТ Р34.10-94\n" << NORMAL;
		cout << CYAN << "4 - Ввести имя нового файла (текущее имя: " << file_name << ")\n" << NORMAL;
		cout << RED << "Q - Выход из программы\n" << NORMAL;
		cout << "Ввод >> ";
		cin >> input;

		size = file_size(file_name);

		switch(input[0]) {
			case '1':
				rsa_sign(file_name, d, n);
				check = rsa_valid(file_name, d, n, size);
				(check) ? cout << "Сертификат действителен.\n" : cout << "НЕ действителен!\n";
				delete_ds(file_name, size);
			break;
			case '2':
				el_gamal_sign(file_name, p, g, y, r);
				check = el_gamal_valid(file_name, p, g, y, r, size);
				(check) ? cout << "Сертификат действителен.\n" : cout << "НЕ действителен!\n";
				delete_ds(file_name, size);
			break;
			case '3':
				gost_sign(file_name, p, q, r, a, y);
				check = gost_valid(file_name, p, q, r, a, y, size);
				(check) ? cout << "Сертификат действителен.\n" : cout << "НЕ действителен!\n";
				delete_ds(file_name, size);
			break;
			case '4':
				cout << "Введите имя файла\nВвод >> ";
				cin >> input;
				if(!check_file(input)) file_name = input;
			break;
			case 'q':
			case 'Q':
			break;
			default:
				cout << RED << "Ошибка! Введены неверные данные.\n" << NORMAL;
			break;
		};
	} while(input[0] != 'q' && input[0] != 'Q');

	return 0;
}
