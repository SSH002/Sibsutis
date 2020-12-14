#include <iterator>
#include <algorithm>
#include "cryptography.hpp"

uint64 get_order(lint number)
{
	return number ? (uint64)bm::ceil(bm::log10(bm::abs(bfloat(number) + (bfloat)0.5))) : 1;
}

string lint_to_str(lint number)
{
	uint64 order = get_order(number);
	lint bit, buff;

	unique_ptr<char[]> buffer(new char [order]);

	for(uint64 i = 1; i <= order; ++i) {
		buff = bm::pow((lint)10, order - i);
		bit = number / buff;
		number = number - bit * buff;
		buffer[i - 1] = char(48 | (uint8)bit);
	}

	string result(buffer.get());
	result.resize(order);
	return result;
}

lint str_to_lint(string number)
{
	lint result = 0;
	uint64 order = number.length();
	uint8 high, low;

	for(uint64 i = 0; i < order; ++i) {
		high = number[i] & 0b00001111;
		result += high;
		result *= 10;
	}

	return result / 10;
}

lint gen_big_prime(uint16 bits)
{
	string param = "openssl prime -generate -bits " + to_string(bits) + " >> buffer";

	system(param.c_str());
	ifstream file("buffer");

	lint buffer;
	file >> buffer;

	file.close();
	system("rm buffer");

	return buffer;
}

//Сервер
server::server()
{
	votes_yes = votes_no = corrupted = 0;

	do {
		p = gen_big_prime(1024);
		q = gen_big_prime(1024);
	} while(p == q);

	n = p * q;
	phi = (p - 1) * (q - 1);

	do {
		d = gen_coprime(phi);
		c = mod_inv(d, phi);
	} while(gcd(c * d, phi) != 1 && c >= p && d >= p);
}

lint server::send_n()
{
	return n;
}

lint server::send_d()
{
	return d;
}

const vector<lint> &server::get_bulletins()
{
	return bulletins;
}

bool server::check_registr(const lint h1)
{
	if(registr.size() == 0) return false;

	vector<lint>::iterator it = find(registr.begin(), registr.end(), h1);

	return it != registr.end();
}

lint server::create_bulletin(const lint h1)
{
	registr.push_back(h1);

	return mod_pow(h1, c, n);
}

bool server::check_bulletin(const lint s, const lint m)
{
	lint h = str_to_lint(sha256(lint_to_str(m)));

	if(h != mod_pow(s, d, n)) {
		++corrupted;
		return false;
	}

	//Подсчёт голосов
	switch(uint8(m)) {
		case 255:
			++votes_yes;
		break;
		case 127:
			++votes_no;
		break;
		default:
			++corrupted;
		break;
	};

	bulletins.push_back(s);
	return true;
}

void server::print_results()
{
	cout << "\nГолосов ЗА: " << votes_yes;
	cout << "\nГолосов ПРОТИВ: " << votes_no;
	cout << "\nИспорченных бюллетеней: " << corrupted << endl;
}

server::~server()
{
}

//Избиратель
constituent::constituent()
{
}

constituent::constituent(const lint n, const lint d)
{
	rnd = gen_number(32);
	v = (rand() % 2) ? 255 : 127;
	m = (rnd << 8) | v;

	r = gen_coprime(n);

	h = str_to_lint(sha256(lint_to_str(m)));
	h1 = h * mod_pow(r, d, n) % n;
}

lint constituent::send_h1()
{
	return h1;
}

void constituent::create_s(const lint bulletin, const lint n)
{
	inv_r = mod_inv(r, n);

	s = bulletin * inv_r % n;
}

lint constituent::send_s()
{
	return s;
}

lint constituent::send_m()
{
	return m;
}

constituent::~constituent()
{
}
