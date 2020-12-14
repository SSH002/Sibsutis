#include <cmath>
#include "cryptography.hpp"
using namespace std;

lint gen_number(uint64 order)
{
	lint number = 2 + rand() % 8;

	for(uint64 i = 0; i < order; ++i) {
		number *= 10;
		number = number + rand() % 10;
	}
	
	return number;
}

lint gen_prime(uint64 order)
{
	lint number;

	do {
		number = gen_number(order);
	} while(!is_prime(number));

	return number;
}

lint gen_number_max(lint maximum)
{
	uint64 order = to_string((uint64)maximum).length();

	lint bit = maximum / bm::pow((lint)10, order - 1);
	lint number;

	if(bit > 1) {
		number = 1 + rand() % (bit - 1);
	} else {
		number = 1;
	}

	for(uint64 i = 2; i <= order; ++i) {
		number *= 10;
		number = number + rand() % 10;
	}

	return number;
}

lint mod_pow(lint num, lint degree, lint p)
{
	lint result = 1;
	for(; degree; num = num * num % p, degree >>= 1) {
		if(degree & 1) {
			result = result * num % p;
		}
	}

	return result;
}

bool is_prime(lint number)
{
	lint sqrt_n = bm::sqrt(number);

	for(uint64 i = 2; i < sqrt_n; ++i) {
		if(number % i == 0) return false;
	}

	return true;
}

lint gcd(lint number1, lint number2)
{
	lint number = gcd(number2 % number1, number1);

	return number;
}

lint gcd(lint number1, lint number2, lint *x, lint *y)
{
	if(!number1) {
		*x = 0;
		*y = 1;

	   	return number2;
	}

	lint number, x1, y1;
	number = gcd(number2 % number1, number1, &x1, &y1);

	*x = y1 - (number2 / number1) * x1;
	*y = x1;

	return number;
}

lint egcd(lint number1, lint number2, lint *x, lint *y)
{
	if(number1 >= number2){
		return gcd(number1, number2, x, y);
	} else {
		return gcd(number2, number1, x, y);
	}
}

lint gen_p()
{
	lint q, p;

	do {
		q = lint(3000000000) + rand() % 2000000000;
		p = 2 * q + 1;
	} while(!is_prime(q) || !is_prime(p));

	return p;
}

lint gen_g(lint p)
{
	uint64 order = to_string((uint64)p).length() - 1;
	
	lint g, q = (p - 1) / 2;

	do {
		g = gen_number(8);
	} while(mod_pow(g, q, p) == 1);

	return g;
}

lint diffy_hellman(lint number1, lint number2, uint64 order)
{
	lint p, g;

	//Генерируем числа p и g
	p = gen_p();
	g = gen_g(p);

	//Генерируем ключи
	lint y1, y2;
	y1 = mod_pow(g, number2, p);
	y2 = mod_pow(g, number1, p);

	lint z12, z21;
	//Шифруем
	z12 = mod_pow(y2, number2, p);
	//Дешифруем
	z21 = mod_pow(y1, number1, p);

	return z12;
}

lint bg_step(lint a, lint y, lint p)
{
	lint ind_i, ind_j;
	lint m_k = bm::sqrt(p) + 1;
	map<lint, lint> Arr_m;
	vector<lint> Arr_k;

	for(uint64 i = 0; i < m_k; ++i) {
		Arr_m[mod_pow(a, i, p) * y % p] = i;
		Arr_k.push_back(mod_pow(a, i * m_k, p));
	}

	for(uint64 i = 0; i < m_k; ++i) {
		if(Arr_m.count(Arr_k[i]) > 0) {
			ind_i = i;
			ind_j = Arr_m[Arr_k[i]];
			break;
		}
	}

	if((ind_i == 0) && (ind_j == 0)) {
		return -1;
	}

	return ind_i * m_k - ind_j;
}
