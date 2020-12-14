#include <regex>
#include <string>
#include <iostream>
#include "cryptography.hpp"

bool is_number(string number)
{
	const regex reg = regex("^[1-9][0-9]*$");

	return regex_search(number.begin(), number.end(), reg);
}

int main(int argc, char *argv[])
{
	srand(time(0));
	string input;

	if(argc > 1) {
		string number(argv[1]);
		if(is_number(number) && (stoi(number) > 1 && stoi(number) < 24)) {
			mental_pocker(stoi(number));
		} else {
			cout << "Ошибка! \"" << number << "\" не является допустимым числом.\n";
			cout << "Пример запуска: \"./main <N>\"; где N[2, 23] - количество игроков\n";
		}
	} else {
		cout << "Ошибка! Некорректные параметры запуска.\n";
		cout << "Пример запуска: \"./main <N>\"; где N[2, 23] - количество игроков\n";
	}

	return 0;
}
