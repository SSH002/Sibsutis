#pragma once
#include <map>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <dirent.h>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
namespace bm = boost::multiprecision;
using namespace std;

typedef unsigned char uint8;
typedef unsigned long long uint64;
typedef bm::cpp_int lint;

#define RED "\033[01;31m"
#define GREEN "\033[01;32m"
#define YELLOW "\033[01;33m"
#define BLUE "\033[01;34m"
#define PURPLE "\033[01;35m"
#define CYAN "\033[01;36m"
#define WHITE "\033[01;37m"
#define NORMAL "\033[0m"

//Лабораторная работа 1
lint gen_number(uint64 order);
lint gen_number_max(lint maximum);
lint gen_prime(uint64 order);

bool is_prime(lint number);
lint mod_pow(lint num, lint degree, lint p);

lint gcd(lint number1, lint number2, lint *x, lint *y);
lint egcd(lint number1, lint number2, lint *x, lint *y);

lint gen_p();
lint gen_g(lint p);
lint diffy_hellman(lint number1, lint number2);

void gen_numbers(lint *a, lint *y, lint *p , uint64 order);
lint bg_step(lint a, lint y, lint p);

//Лабораторная работа 2
uint8 check_file(string filename);
lint *read_enc(string filename, uint64 *length);
string read_dec(string filename);
void write_enc(string filename, lint *enc, uint64 length);
void write_dec(string filename, lint *dec, uint64 length);

lint gen_coprime(lint prime);
lint mod_inv(lint private1, lint p);

void shamir_encrypt(string filename);
void shamir_decrypt(string filename);

void el_gamal_encrypt(string filename);
void el_gamal_decrypt(string filename);

uint8 count_1bytes(uint8 symbol);
void vernam_encrypt(string filename);
void vernam_decrypt(string filename);

void rsa_encrypt(string filename);
void rsa_decrypt(string filename);

//Лабораторная работа 3

