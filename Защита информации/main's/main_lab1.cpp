#include <string>
#include <iostream>
#include "cryptography.hpp"
using namespace std;

int main()
{
	srand(time(NULL));

	//Расширенный алгоритм Евклида
	lint x, y;
	lint number = gen_number(9);
	lint number2 = gen_number(9);
	lint number3;

	printf("%sALGORITHM GCD%s\n", RED, NORMAL);
	cout << "{" << number << " || " << number2 << "}" << endl;
	cout << "GCD = " << egcd(number, number2, &x, &y) << ", x = " << x << ", y = " << y << endl << endl;

	//Быстрое возведение в степень
	number3 = gen_prime(9);
	number = gen_number_max(number3);
	number2 = gen_number_max(number3);

	printf("%sALGORITHM MOD_POW%s\n", YELLOW, NORMAL);
	cout << "{" << number << " || " << number2 << " || " << number3 << "}" << endl;
	cout << "Mod_Pow = " << mod_pow(number, number2, number3) << endl << endl;

	//Алгоритм Диффи-Хеллмана
	number = gen_number(9);
	number2 = gen_number(9);

	printf("%sALGORITHM DIFFY-HELLMAN%s\n", GREEN, NORMAL);
	cout << "{" << number << " || " << number2 << "}" << endl;
	lint numb = diffy_hellman(number, number2);
	cout << "Diffy-Hellman = " << numb << endl << endl;

	//Алгоритм "Шаг младенца - шаг великана"
	number3 = gen_prime(9);
	number = gen_number_max(number3);
	number2 = gen_number_max(number3);

	printf("%sALGORITHM BABY-STEP-GIANT-STEP%s\n", BLUE, NORMAL);
	cout << "{" << number << " || " << number2 << " || " << number3 << "}" << endl;
	lint answer = bg_step(number, number2, number3);

	if(answer > 0) {
		cout << "BG_Step = " << answer << endl;
	} else {
		cout << "Пример не решаем.\n";
	}

	return 0;
}

