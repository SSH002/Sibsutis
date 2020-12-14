#pragma once
#include <map>
#include <array>
#include <cmath>
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <dirent.h>
#include <iostream>
#include <sha256.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
namespace bm = boost::multiprecision;
using namespace std;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long long uint64;
typedef boost::multiprecision::cpp_int lint;
typedef boost::multiprecision::cpp_dec_float_50 bfloat;

#define RED "\033[01;31m"
#define GREEN "\033[01;32m"
#define YELLOW "\033[01;33m"
#define BLUE "\033[01;34m"
#define PURPLE "\033[01;35m"
#define CYAN "\033[01;36m"
#define WHITE "\033[01;37m"
#define NORMAL "\033[0m"
constexpr uint8 SHA256_HL = 32;

//Лабораторная работа 1
lint gen_number(uint64 order);
lint gen_number_max(lint maximum);
lint gen_prime(uint64 order);
bool is_prime(lint number);

lint mod_pow(lint num, lint degree, lint p);

lint gcd(lint number1, lint number2);
lint gcd(lint number1, lint number2, lint *x, lint *y);
lint egcd(lint number1, lint number2, lint *x, lint *y);

lint gen_p();
lint gen_g(lint p);
lint diffy_hellman(lint number1, lint number2);
lint dh_enc(lint *p, lint *g, lint number1, lint number2);
lint dh_dec(lint p, lint g, lint number1, lint number2);

lint bg_step(lint a, lint y, lint p);

//Лабораторная работа 2
uint8 check_file(string file_name);
lint *read_enc(string file_name, uint64 *length);
string read_dec(string file_name);
void write_enc(string file_name, lint *enc, uint64 length);
void write_dec(string file_name, lint *dec, uint64 length);

lint gen_coprime(lint prime);
lint mod_inv(lint priv1, lint p);
void shamir_enc(string file_name);
void shamir_dec(string file_name);

void el_gamal_enc(string file_name);
void el_gamal_dec(string file_name);

uint8 binary_size(uint8 byte);
void vernam_enc(string file_name);
void vernam_dec(string file_name);

void rsa_enc(string file_name);
void rsa_dec(string file_name);

//Лабораторная работа 3
string read_dec(string file_name, uint64 length);
array<lint, 32> read_ds(string file_name, uint64 size);
void write_ds(string file_name, array<lint, 32> S);
void delete_ds(string file_name, uint64 size);

array<uint8, 32> sha256_to_bytes(string Hash);
lint mod(lint n, lint d);

void rsa_sign(string file_name, lint &d, lint &n);
bool rsa_valid(string file_name, lint d, lint n, uint64 size);

void el_gamal_sign(string file_name, lint &p, lint &g, lint &y, lint &r);
bool el_gamal_valid(string file_name, lint p, lint g, lint y, lint r, uint64 size);

void gost_sign(string file_name, lint &p, lint &q, lint &r, lint &a, lint &y);
bool gost_valid(string file_name, lint p, lint q, lint r, lint a, lint y, uint64 size);

//Лабораторная работа 4
void mental_pocker(const uint8 players);
void reverse_cards(vector<lint> &U);
void print_cards(const vector<vector<lint>> Cards, const uint8 players, bool is_put[52]);

//Лабораторная работа 5
uint64 get_order(lint number);											//Получаем длину числа *****(перенести в 1-ую лабу)*****
string lint_to_str(lint number);										//Конверсия длинного числа в строку
lint str_to_lint(string number);										//Конверсия строки в длинное число
lint gen_big_prime(uint16 bits);										//Для её работы требуется библиотека "openssl"

class server
{
	private:
		lint p, q, n, phi;												//Константы
		lint c, d;														//Закрытый и открытый ключи

		uint64 votes_yes, votes_no;										//Подсчёт голосов
		uint64 corrupted;												//Количество недействительных бюллетеней

		vector<lint> registr;											//Реестр избирателей
		vector<lint> bulletins;											//БД зарегистрированных бюллетеней

	public:
		server();														//Генерация констант и ключей
		lint send_n();													//Публикация числа N
		lint send_d();													//Публикация открытого ключа

		const vector<lint> &get_bulletins();							//Публикация списка бюллетеней
		bool check_registr(lint h_1);									//Проверка, не голосовал ли избиратель ранее

		lint create_bulletin(lint h_1);									//Создание бюллетеня
		bool check_bulletin(lint s, lint m);							//Проверка подписи бюллетеня

		void print_results();											//Вывести результаты голосования

		~server();
};

class constituent
{
	private:
		lint rnd;														//Префикс
		lint v;															//Голос избирателя
		lint m;															//m = rnd | v
		lint r;															//Взаимно простое с n
		lint inv_r;														//Инверсия r по модулю n
		lint h, h1;														//Хеш и его инверсия
		lint s;															//Подпись избирателя

	public:
		constituent();
		constituent(const lint n, const lint d);						//Генерация чисел избирателя

		void create_s(lint bulletin, lint n);							//Подписание бюллетеня

		lint send_h1();													//Отправляем серверу число h1 
		lint send_s();													//Отправляем серверу число s
		lint send_m();													//Отправляем серверу число m

		~constituent();
};
